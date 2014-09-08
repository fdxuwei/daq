#ifndef _ITEM_MODBUS_H_
#define _ITEM_MODBUS_H_

#include <string>
#include "item.h"

class item_modbus: public item
{
	friend class acqer_modbus;
public:
	item_modbus(int id, const std::string &name, unsigned cycle, const std::string &expr, ITEM_VALUE_TYPE ivt, bool onlyexpr = true, uint8 funcode = 0, uint16 startaddr = 0, uint8 sa_offset = 0, uint16 count = 0);
	
	/* acq method */
	uint8 funcode_;
	uint16 startaddr_;
	uint8 sa_offset_;
	uint16 count_;
};

#endif

