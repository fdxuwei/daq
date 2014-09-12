#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "arpa/inet.h"
#include "acqer_modbus.h"
#include "comm.h"

using namespace std;

acqer_modbus::acqer_modbus(uint8 devaddr)
	: protomod_ (NULL)
	, devaddr_ (devaddr) 
{
}

acqer_modbus::~acqer_modbus()
{
	delete protomod_;
	protomod_ = NULL;
}

void acqer_modbus::add_item(item *ip)
{
	item_modbus *imp = dynamic_cast<item_modbus*>(ip);
	assert(imp != NULL);
	acqer::add_item(ip);
	if(!imp->onlyexpr_)
		fc_items_[imp->funcode_].push_back(imp);
}

void acqer_modbus::simple_acq()
{
	uint8 reqbuf[MODBUS_MAX_REQUEST_LEN];
	uint16 reqlen = sizeof(reqbuf);
	/**/
	acqtime_ = time(NULL);
	/* clear item status */
	for(item_list::iterator it = items_.begin(); it != items_.end(); ++it)
	{
		/* is the acq time */
		if(is_acqtime(*it))
		{	
			(*it)->status_ = IS_FAILED;
		}
	}
	/* begin acq */
	fc_item_itr_ = fc_items_.begin();
	item_begin_itr_ = item_end_itr_ = fc_item_itr_->second.begin();
	while(make_req(reqbuf, reqlen) > 0)
	{
		uint16 rsplen = protomod_->GetRspBufSize(req_funcode_, req_count_);
		uint8 rspbuf[MODBUS_MAX_RESPONSE_LEN];
		/* io */
		comm *commp = init_comm();
		if(NULL == commp)
		{
			//
			break;
		}
		if(commp->write(reqbuf, reqlen) != reqlen)
		{
			break;
		}
		if(commp->read(rspbuf, rsplen) != rsplen)
		{
			break;
		}
		/* parse */
		uint8 devaddr;
		uint8 funcode;
		uint8 *pout;
		uint16 outlen;
		if(!protomod_->ParsePollingRsp(rspbuf, rsplen, devaddr, funcode, pout, outlen))
		{
			/* checksum err */
			break;
		}
		memcpy(frame_data_, pout, outlen);
		frame_data_len_ = outlen;

		/* make item values */
		make_item_values();

	}
}

size_t acqer_modbus::make_req(void *buf, uint16 &len)
{
	if(item_end_itr_ == fc_item_itr_->second.end())
	{
		if((++fc_item_itr_) != fc_items_.end())
			item_begin_itr_ = item_end_itr_ = fc_item_itr_->second.begin();
		else
			return 0; /* cycle request completed */
	}
	// skip non acqtimt, and only-expression items
	while((item_end_itr_ != fc_item_itr_->second.end())	&& ((!is_acqtime(*item_end_itr_)) || ((*item_end_itr_)->onlyexpr_)))
	{
		++item_end_itr_;
	}
	//
	item_begin_itr_ = item_end_itr_;
	/* split request, as we may not be able to get all the frame once.
	 onyl little than, not equal, leave at least one byte empty space.*/
	while(item_end_itr_ != fc_item_itr_->second.end())
	{
		item_modbus* endi = *item_end_itr_;
		/* is not the acq time, or only expression items */
		if((!is_acqtime(endi)) || (endi->onlyexpr_))
		{
			++item_end_itr_;
			continue;
		}
		if(((*item_end_itr_)->startaddr_ - (*item_begin_itr_)->startaddr_ + (*item_end_itr_)->count_)*2 >= MODBUS_MAX_FRAME_DATA_LEN)
			break;
		 ++item_end_itr_;
	}
	// no item to acq
	if(item_begin_itr_ == fc_item_itr_->second.end())
		return 0;
	/* make request*/
	item_modbus_list::const_iterator item_last_itr = item_end_itr_;
	item_last_itr--;
	req_funcode_ = (*item_begin_itr_)->funcode_;
	req_startaddr_ = (*item_begin_itr_)->startaddr_;
	req_count_ = (*item_last_itr)->startaddr_ - (*item_begin_itr_)->startaddr_ + (*item_last_itr)->count_;
	protomod_->PackPollingReq(devaddr_, req_funcode_, req_startaddr_, req_count_, (uint8*)buf, len);
	return len;
}

void acqer_modbus::make_item_values()
{
	while(item_begin_itr_ != item_end_itr_)
	{
		item_modbus *pim = *item_begin_itr_;
		/* it is not the acq time, or only expression items*/
		if((!is_acqtime(pim)) || (pim->onlyexpr_))
		{
			++item_begin_itr_;
			continue;
		}
		/* configuration must be correct */
		assert(pim->sa_offset_ == 0 ||
				(pim->sa_offset_ == 1 && pim->count_ == 1));
		char *val_addr = frame_data_ + 2*(pim->startaddr_ - req_startaddr_);
		switch(pim->value_.ivt_)
		{
			case IVT_INTEGER:
				/* big endian */
				if(pim->sa_offset_ == 1)
					pim->value_.val_.ival = *(int8*)(val_addr + pim->sa_offset_);
				else if(pim->count_ == 1)
					pim->value_.val_.ival = htons(*(int16*)val_addr);
				else if(pim->count_ == 2)
					pim->value_.val_.ival = htonl(*(int32*)val_addr);
				else
					assert(false);
				break;
			case IVT_FLOAT:
				if(pim->count_ == 2)
					pim->value_.val_.fval = *(float*)val_addr;
				else if(pim->count_ == 4)
					pim->value_.val_.fval = *(double*)val_addr;		
				else
					assert(false);
				break;
			case IVT_STRING:
				assert(pim->count_*2 < ITEM_MAX_STRING_LEN);
				memcpy(pim->value_.val_.sval, val_addr, pim->count_*2);
				pim->value_.val_.sval[pim->count_*2] = 0;
				break;
			default: /* it's not possible */
				assert(false);
		}
		pim->status_ = IS_UPDATED;
		++item_begin_itr_;
	}
}
