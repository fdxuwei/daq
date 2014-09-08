#ifndef _OPERATOR_FACTORY_H_
#define _OPERATOR_FACTORY_H_

#include <string>
#include "basic_operator.h"
#include "math_operator.h"

class operator_factory
{
public:
	static basic_operator* create(char opch);
	static basic_operator* create(const std::string &ops);

};


#endif

