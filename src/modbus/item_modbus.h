#ifndef _ITEM_MODBUS_H_
#define _ITEM_MODBUS_H_

#include <string>

class item_modbus
{
	friend class acqer_modbus;
public:
protected:
	int id_;
	std::string name_;
	unsigned cycle_;
	
	/* acq method */
	unsigned char funcode_;
	unsigned short startbyte_;
	unsigned short bytes_;
	std::string expr_;
	/* value  */
	variant value_;
	/* acq status */
	bool status_;

};

#endif

