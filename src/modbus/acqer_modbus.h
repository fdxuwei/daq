#ifndef _ACQER_MODBUS_H_
#define _ACQER_MODBUS_H_

#include <list>
#include <map>

#define MODBUS_MAX_FRAME_DATA_LEN 256
#define MODBUS_MAX_REQUEST_LEN 20


enum ITEM_VALUE_TYPE
{
	IVT_INTEGER,
	ITV_FLOAT,
	ITV_STRING
};

class acqer_modbus
{
public:
	~acqer_modbus();
	void acq_once();
protected:
	size_t read();
	size_t write();
private:
	typedef std::list<item_modbus*> item_list;
	typedef std::map<unsigned char, item_list> fc_item_map;
	/* every cycle of  acquisition may cost several requests.
	 items begin item_begin_itr_ and item_end_itr_ have the same funcode. */
	size_t make_req(void *buf, size_t &len);
	fc_item_map::const_iterator	fc_item_itr_;
	item_list::const_iterator item_begin_itr_;
	item_list::const_iterator item_end_itr_;
	/* make item values from frame_data_  */
	void make_item_values();
	/* acq  datas */
	char frame_data_[MAX_FRAME_DATA_LEN];
	size_t frame_data_len_;
	unsigned short frame_startbyte_;
	/* items */
	item_list items_;
	/* fc_items_, mapped by funcode_, 
	   and each map list is ordered by startbyte_ */
	fc_item_map fc_items_;
};

#endif

