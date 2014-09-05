#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <string>

void err_exit(const char *fmt, ...);

int str2int(const std::string & str);
unsigned int str2uint(const std::string & str);

#endif

