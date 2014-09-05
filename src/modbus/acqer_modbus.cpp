#include "acqer_modbus.h"
#include "comm.h"

using namespace std;

void acqer_modbus::add_item(item *ip)
{
	item_modbus *imp = dynamic_cast<item_modbus*>(ip);
	assert(imp != NULL);
	acqer::add_item(ip);
	fc_items_[ip->funcode].push_back(ip);
}

void acqer_modbus::acq_once()
{
	char reqbuf[MODBUS_MAX_REQUEST_LEN];
	uint16 reqlen = sizeof(reqbuf);
	/* clear item status */
	for(item_list::iterator it = items_.begin(); it != items_.end(); ++it)
		(*it)->status_ = false;
	/* begin acq */
	fc_item_itr_ = fc_item_.begin();
	item_begin_itr_ = item_end_itr_ = fc_item_itr_->second.begin();
	while(make_req(reqbuf, reqlen) > 0)
	{
		uint16 rsplen = proto_->GetRspBufSize(req_funcode_, req_count_);
		char *rspbuf = new char[rsplen];
		/* io */
		comm *commp = init_comm();
		if(NULL == commp)
		{
			//
			return;
		}
		if(comm->write(reqbuf, reqlen) != reqlen)
		{
			break;
		}
		if(comm->read(rspbuf, rsplen) != rsplen)
		{
			break;
		}
		/* parse */
		uint8 devaddr;
		uint8 funcode;
		uint8 *pout;
		uint16 outlen;
		if(!proto_->ParsePollingRsp(rspbuf, rsplen, devaddr, funcode, pout, outlen))
		{
			/* checksum err */
		}
		memcpy(frame_data_, pout, outlen);
		frame_data_len_ = outlen;

		/* make item values */
		make_item_values();

		delete []rspbuf;
	}
}

size_t acqer_modbus::make_req(void *buf, uint16 &len)
{
	if(item_end_itr_ == item_list::end())
	{
		if((++fc_item_itr_) != fc_items_.end())
			item_begin_itr_ = item_end_itr_ = fc_item_itr_->second->begin();
		else
			return 0; /* cycle request completed */
	}
	item_begin_itr_ = item_end_itr_;
	/* split request, as we may not be able to get all the frame once.
	 onyl little than, not equal, leave at least one byte empty space.*/
	while(item_end_itr_ != list<item_modbus*>::end()
			&& ((*item_end_itr_)->startaddr_ - (*item_begin_itr)->startaddr_ + (*item_end_itr_)->count_)*2 < MODBUS_MAX_FRAME_DATA_LEN)
	{ ++item_end_itr;}

	/* make request*/
	item_modbus_list::const_iterator item_last_itr = item_end_itr_;
	item_last_itr--;
	req_funcode_ = (*item_begin_itr_)->funcode_;
	req_startaddr_ = (*item_begin_itr_)->startaddr_;
	req_count_ = (*item_last_itr)->startaddr_ - (*item_begin_itr_)->startaddr_ + (*item_last_itr)->count_;
	proto_->PackPollingReq(devaddr_, req_funcode_, req_startaddr_, req_count_, buf, len);
	return len;
}

void acqer_modbus::make_item_values()
{
	while(item_begin_itr_ != item_end_itr_)
	{
		item_modbus *pim = *item_begin_itr_;
		/* configuration must be correct */
		assert(pim->sa_offset_ == 0 ||
				(pim->sa_offset_ == 1 && pim->bytes_ == 1));
		char *val_addr = frame_data + pim->startaddr_ - req_startaddr_;
		switch(pim->value_.ivt_)
		{
			case ITV_INTEGER:
				if(pim->bytes_ == 1)
					pim->value_.ival_ = *(int8*)(val_addr + pim->sa_offset_);
				else if(pim->bytes_ == 2)
					pim->value_.ival_ = *(int16*)val_addr;
				else if(pim->bytes_ == 4)
					pim->value_.ival_ = *(int32*)val_addr;
				else
					assert(false);
				break;
			case ITV_FLOAT:
				if(pim->bytes_ == 4)
					pim->value_.fval_ = *(float*)val_addr;
				else if(pim->bytes_ == 8)
					pim->value_.fval_ = *(double*)val_addr;		
				else
					assert(false);
				break;
			case ITV_STRIING:
				pim->value_.sval_ = string(val_addr, pim->bytes_);
				break;
			default: /* it's not possible */
				assert(false);
		}
		pim->status_ = true;
		++item_begin_itr_;
	}
}
