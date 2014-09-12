#include <iostream>
#include <assert.h>
#include <time.h>
#include "item_handler.h"

using namespace std;

item_handler::item_handler()
{}

void item_handler::handle_item(const string &devid, item *ip)
{
	cout << time(NULL) << ":" << devid << ":"<< ip->name_ << ":";
	if(ip->status_ == IS_FAILED)
	{
		cout << "false" << endl;
		ip->status_ = IS_OLD;
		return;
	}
	itemval_t *ivp = &(ip->value_);
	switch(ivp->ivt_)
	{
		case IVT_INTEGER:
			cout << ivp->val_.ival;
			break;
		case IVT_FLOAT:
			cout << ivp->val_.fval;
			break;
		case IVT_STRING:
			cout << ivp->val_.sval;
			break;
		default:
			assert(false);
	}
	cout << endl;
	ip->status_ = IS_OLD;
}
