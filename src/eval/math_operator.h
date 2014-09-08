#ifndef _MATH_OPERATOR_H_
#define _MATH_OPERATOR_H_

#include <math.h>
#include "basic_operator.h"

/* + */
class operator_add : public basic_operator
{
public:
	operator_add(){argnum_ = 2; }
	virtual double calc(double arg1, double arg2) { return arg1+arg2; }
};

/* - */
class operator_sub : public basic_operator
{
public:
	operator_sub(){argnum_ = 2; }
	virtual double calc(double arg1, double arg2) { return arg1-arg2; }
};

/* * */
class operator_mul : public basic_operator
{
public:
	operator_mul(){argnum_ = 2; }
	virtual double calc(double arg1, double arg2) { return arg1*arg2; }
};


/* / */
class operator_div : public basic_operator
{
public:
	operator_div(){argnum_ = 2; }
	virtual double calc(double arg1, double arg2) { return arg1/arg2; }
};

/* % */
class operator_mod : public basic_operator
{
public:
	operator_mod(){argnum_ = 2; }
	virtual double calc(double arg1, double arg2) { return (double)((long)arg1%(long)arg2); }
};

/* ^ */
class operator_pow : public basic_operator
{
public:
	operator_pow(){argnum_ = 2; }
	virtual double calc(double arg1, double arg2) { return pow(arg1,arg2); }
};

/* sqrt */
class operator_sqrt : public basic_operator
{
public:
	operator_sqrt(){argnum_ = 1; }
	virtual double calc(double arg) { return sqrt(arg); }
};

/* sin */
class operator_sin : public basic_operator
{
public:
	operator_sin(){argnum_ = 1; }
	virtual double calc(double arg) {return sin(arg); }
};

/* cos */
class operator_cos : public basic_operator
{
public:
	operator_cos(){argnum_ = 1; }
	virtual double calc(double arg) {return cos(arg); }
};

/* lg , log10 */
class operator_lg : public basic_operator
{
public:
	operator_lg(){argnum_ = 1; }
	virtual double calc(double arg) {return log10(arg); }
};

/* ln, nature logarithm  */
class operator_ln : public basic_operator
{
public:
	operator_ln(){argnum_ = 1; }
	virtual double calc(double arg) {return log(arg); }
};

#endif

