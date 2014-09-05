#include "dev_pool.h"

using namespace std;

dev_pool::~dev_pool()
{
	for(vector<dev*>::iterator it = devs_.begin(); it != devs_.end(); ++it)
	{
		delete (*it);
	}
}

void dev_pool::add_dev(dev *dp)
{
	devs_.push_back(dp);
}

void dev_pool::acq_once()
{
	for(vector<dev*>::iterator it = devs_.begin(); it != devs_.end(); ++it)
	{
		(*it)->acq_once();
	}
}

