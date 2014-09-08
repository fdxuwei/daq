#include "system.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void err_exit(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	if(errno)
		printf(": %s", strerror(errno));
	printf("\n");
	va_end(args);
	exit(errno);
}

int str2int(const std::string & str)
{
	if(str.empty())
		return 0;
	else
		return atoi(str.c_str());
}

unsigned int str2uint(const std::string & str)
{
	if(str.empty())
		return 0;
	else
		return strtoul(str.c_str(), NULL, 10);
}
