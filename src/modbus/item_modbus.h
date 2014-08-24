#ifndef _ITEM_MODBUS_H_
#define _ITEM_MODBUS_H_

#include <string>

enum ITEM_VALUE_TYPE
{
    IVT_INTEGER,
    ITV_FLOAT,
    ITV_STRING
};

typedef struct
{
	ITEM_VALUE_TYPE ivt_;
	union
	{
		int32 ival;
		double fval;
		std::string sval;
	}val_;
}

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
	unsigned short startaddr_;
	unsigned char sa_offset_;
	unsigned short bytes_;
	std::string expr_;
	/* value  */
	variant value_;
	/* acq status */
	bool status_;

};

#endif

