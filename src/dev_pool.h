#ifndef _DEV_POOL_H_
#define _DEV_POLL_H_

#include <vector>
#include "dev.h"

class dev_pool
{
public:
	~dev_pool();
	static dev_pool &instance()
	{
		static dev_pool dp;
		return dp;
	}
	void add_dev(dev *dp);
	void acq_once();
private:
	dev_pool();
	std::vector<dev*> devs_;
};

#endif

