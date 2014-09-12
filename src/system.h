#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <string>
#include <sstream>

void err_exit(const char *fmt, ...);

int str2int(const std::string & str);
unsigned int str2uint(const std::string & str);

template<class T>
std::string tostr(T val)
{
	std::stringstream ss;
	ss << val;
	return ss.str();
}

#endif

