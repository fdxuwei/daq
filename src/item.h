#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>
#include "daqtypes.h"

enum ITEM_VALUE_TYPE
{
	IVT_INTEGER,
	IVT_FLOAT,
	IVT_STRING
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
}itemval_t;

class item
{
public:
	item(int id, const std::string &name, unsigned cycle, const std::string &expr);
	virtual ~item();
protected:
	int id_;
	std::string name_;
	unsigned cycle_;
	bool onlyexpr_;
	std::string expr_;
	/* value */
	itemval_t value_;
	/* acq status */
	bool status_;
};

#endif

