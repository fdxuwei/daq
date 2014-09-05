#ifndef _DEV_H_
#define _DEV_H_

#include <string>
#include <vector>
#include "acqer.h"

class dev
{
public:
	dev(const std::string &name);
	~dev();
	void acqer_once();
	void add_acqer(acqer *ap);
private:
	std::string name_;
	std::vector<acqer*> acqers_;
};

#endif

