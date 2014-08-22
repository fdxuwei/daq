#include "acqer_modbus.h"

using namespace std;

size_t acqer_modbus::make_req(void *buf, size_t &len)
{
	if(item_end_itr_ == item_list::end())
	{
		if((++fc_item_itr_) != fc_items_.end())
			item_begin_itr_ = item_end_itr_ = fc_item_itr_->second->begin();
		else
			return 0; /* cycle request completed */
	}
	item_begin_itr_ = item_end_itr_;
	/* split request, as we may not be able to get all the frame once */
	while(item_end_itr_ != list<item_modbus*>::end()
			&& ((*item_end_itr_)->startbyte_ + (*item_end_itr_)->bytes_ - frame_startbyte_ - frame_data_len_) <= MODBUS_MAX_FRAME_DATA_LEN)
	{}

	/* make request*/
	list<item_modbus*>::const_iterator item_last_itr = item_end_itr_;
	item_last_itr--;
	unsigned char funcode = (*item_begin_itr_)->funcode_;
	unsigned short startbyte = frame_startbyte_ + frame_data_len;
	unsigned short bytes = (*item_last_itr)->startbyte_ + (*item_last_itr)->bytes_ - (*item_begin_itr_)->startbyte_;
	;
}


