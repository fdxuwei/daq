#ifndef _ACQER_H_
#define _ACQER_H_

#include <string>
#include <vector>
#include <map>
#include <time.h>
#include "item.h"

typedef std::vector<item*> item_list;
typedef std::map<int, item*> item_map;

class acqer
{
public:
	acqer();
	virtual ~acqer();
	virtual void add_item(item *ip);
	void acq_once();
	item_list &get_itemlist() {return items_; }
	bool calc_items(); // handle expressions
protected:
	virtual void simple_acq() = 0;
	bool is_acqtime(item *ip);
	void update_item_acqtimes();
	std::string proto_;
	item_list items_;
	item_map iditem_;
	time_t acqtime_;
private:
	bool find_var(const std::string &expr, size_t &varbeg, size_t &varend, int *itemid);
	bool replace_var(std::string &expr, bool *deperr);
};

#endif

