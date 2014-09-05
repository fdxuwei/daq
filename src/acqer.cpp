#include "acqer.h"

using namespace std;

acqer::~acqer()
{
	for(item_list:iterator it = items_.begin(); it != items_.end(); ++it)
	{
		delete (*it);
	}
}


void acqer::add_item(item *ip)
{
	items_.push_back(ip);
}

