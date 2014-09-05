#ifndef _ACQER_H_
#define _ACQER_H_

#include <string>
#include <vector>
#include "item.h"

class acqer
{
public:
	virtual ~acqer();
	virtual void acq_once() = 0;
	virtual void add_item(item *ip);
protected:
	typedef std::vector<item*> item_list;
	std::string proto_;
	item_list items_;
};

#endif

