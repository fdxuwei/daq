#include "dev.h"
#include "item_handler.h"

using namespace std;

dev::dev(const string &name)
	:name_ (name)
{

}

dev::~dev()
{
	for(vector<acqer*>::iterator it = acqers_.begin(); it != acqers_.end(); ++it)
	{
		delete (*it);
	}
}

void dev::add_acqer(acqer *ap)
{
	acqers_.push_back(ap);
}

void dev::acq_once()
{
	for(vector<acqer*>::iterator it = acqers_.begin(); it != acqers_.end(); ++it)
	{
		(*it)->acq_once();
	}
}

void dev::handle_item()
{
	for(vector<acqer*>::iterator ita = acqers_.begin(); ita != acqers_.end(); ++ita)
	{
		item_list &il = (*ita)->get_itemlist();
		for(item_list::iterator iti = il.begin(); iti != il.end(); ++iti)
		{
			item* ip = *iti;
			if(!ip->name_.empty() && (ip->status_ != IS_OLD))
				item_handler::instance().handle_item(name_, ip);
		}
	}
}
