#ifndef _ACQER_MODBUS_H_
#define _ACQER_MODBUS_H_

#include <list>
#include <map>
#include <time.h>

#include "acqer.h"
#include "ProtoModbus.h"
#include "item_modbus.h"

#define MODBUS_MAX_FRAME_DATA_LEN 256
#define MODBUS_MAX_REQUEST_LEN 20

class comm;

class acqer_modbus: public acqer
{
public:
	acqer_modbus(uint8 devaddr);
	~acqer_modbus();
	virtual void acq_once();
	virtual void add_item(item *ip);
protected:
	virtual comm* init_comm() = 0;
	/* modbus protocol */
	CProtoModbus *protomod_;
private:
	typedef std::list<item_modbus*> item_modbus_list;
	typedef std::map<unsigned char, item_modbus_list> fc_item_map;
	/* every cycle of  acquisition may cost several requests.
	 items begin item_begin_itr_ and item_end_itr_ have the same funcode. */
	size_t make_req(void *buf, uint16 &len);
	fc_item_map::const_iterator	fc_item_itr_;
	item_modbus_list::const_iterator item_begin_itr_;
	item_modbus_list::const_iterator item_end_itr_;
	uint8 req_funcode_;
	uint16 req_startaddr_;
	uint16 req_count_;
	/* make item values from frame_data_  */
	void make_item_values();
	/* acq  datas */
	char frame_data_[MODBUS_MAX_FRAME_DATA_LEN];
	size_t frame_data_len_;
	/* modbus devaddr */
	uint8 devaddr_;
	/* fc_items_, mapped by funcode_, 
	   and each map list is ordered by startbyte_ */
	fc_item_map fc_items_;
	/**/
	time_t acqtime_;
};

#endif

