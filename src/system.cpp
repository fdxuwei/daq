#include "system.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>

void err_sys(const char * arg)
{
	printf("%s: %s\n", arg, strerror(errno));
	exit(errno);
}
