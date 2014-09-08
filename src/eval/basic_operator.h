#ifndef _BASIC_OPERATOR_H_
#define _BASIC_OPERATOR_H_

#include <list>
#include <string>

class basic_operator
{
public:
	virtual ~basic_operator(){}
	virtual double calc(double arg){return 0.0; }
	virtual double calc(double arg1, double arg2){return 0.0; }
	virtual double calc(double arg1, double arg2, double arg3){return 0.0; }
	int argnum(){return argnum_; };
protected:
	int argnum_;
private:
};

#endif

