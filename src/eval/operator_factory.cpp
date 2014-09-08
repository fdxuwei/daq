#include "operator_factory.h"
#include <stdio.h>

using namespace std;

basic_operator* operator_factory::create(char opch)
{
	switch(opch)
	{
		case '+':
			return new operator_add;
		case '-':
			return new operator_sub;
		case '*':
			return new operator_mul;
		case '/':
			return new operator_div;
		case '%':
			return new operator_mod;
		case '^':
			return new operator_pow;
		default:
			return NULL;
	}
}

basic_operator* operator_factory::create(const string &ops)
{
	if(ops == "sqrt")
	{
		return new operator_sqrt;
	}
	else if(ops == "pow")
	{
		return new operator_pow;
	}
	else if(ops == "sin")
	{
		return new operator_sin;
	}
	else if(ops == "cos")
	{
		return new operator_cos;
	}
	else if(ops == "lg")
	{
		return new operator_lg;
	}
	else if(ops == "ln")
	{
		return new operator_ln;
	}
	else
	{
		return NULL;
	}
}
