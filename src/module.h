#ifndef _MODULE_H_
#define _MODULE_H_

#include <string>

class module
{
public:
	module(const std::string& newname);
	~module();
	std::string name() const;
	bool run();
	bool read();
	bool write();
private:
	std::string name_;
	int rfd_;
	int wfd_;

};

#endif

