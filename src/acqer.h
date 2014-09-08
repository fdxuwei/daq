#ifndef _ACQER_H_
#define _ACQER_H_

#include <string>
#include <vector>
#include "item.h"

typedef std::vector<item*> item_list;

class acqer
{
public:
	virtual ~acqer();
	virtual void acq_once() = 0;
	virtual void add_item(item *ip);
	item_list &get_itemlist() {return items_; }
protected:
	std::string proto_;
	item_list items_;
};

#endif

