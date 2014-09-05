#include "dev.h"

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

void dev::acqer_once()
{
	for(vector<acqer*>::iterator it = acqers_.begin(); it != acqers_.end(); ++it)
	{
		(*it)->acqer_once();
	}
}

