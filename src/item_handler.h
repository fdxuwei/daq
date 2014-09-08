#ifndef _ITEM_HANDLER_H_
#define _ITEM_HANDLER_H_

#include <string>
#include "item.h"

class item_handler
{
public:
	static item_handler &instance()
	{
		static item_handler ih;
		return ih;
	}

	void handle_item(const std::string &devid, item *ip);
private:
	item_handler();
};

#endif

