#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>
#include "daqtypes.h"

#define ITEM_MAX_STRING_LEN 50

enum ITEM_VALUE_TYPE
{
	IVT_INTEGER,
	IVT_FLOAT,
	IVT_STRING
};

struct itemval_t
{
	ITEM_VALUE_TYPE ivt_;
	union
	{
		int32 ival;
		double fval;
		char sval[ITEM_MAX_STRING_LEN];
	}val_;

};

enum ITEM_STATUS
{
	IS_OLD,
	IS_UPDATED,
	IS_FAILED
};

class item
{
public:
	item(int id, const std::string &name, unsigned cycle, const std::string &expr, ITEM_VALUE_TYPE ivt);
	virtual ~item();
    // member variable	
	int id_;
	std::string name_;
	unsigned cycle_;
	bool onlyexpr_;
	std::string expr_;
	/* value */
	itemval_t value_;
	/* acq status */
	ITEM_STATUS status_;
	/* last acq time */
	time_t latime_;
};

#endif

