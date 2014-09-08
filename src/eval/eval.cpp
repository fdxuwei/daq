#include <deque>
#include <list>
#include <iostream>
#include <typeinfo>
#include <assert.h>
#include "eval.h"
#include "operator_factory.h"

using namespace std;

enum OPTOR_TYPE
{
	OT_OPERATOR,
	OT_OPERAND,
};

/* optor type, include operator, operand */
struct optor
{
	static optor* create(char opr)
	{
		basic_operator *bop = operator_factory::create(opr); 
		if(bop == NULL)
			return NULL;
		optor *op = new optor;
		op->val.operatorp = bop;
		op->type = OT_OPERATOR; 
		return op;
	}
	static optor* create(const string &opr)
	{
		basic_operator *bop = operator_factory::create(opr); 
		if(bop == NULL)
			return NULL;
		optor *op = new optor;
		op->val.operatorp = bop;
		op->type = OT_OPERATOR; 
		return op;
	}
	static optor* create(double opn)
	{
		optor *op = new optor;
		op->val.operand = opn;
		op->type = OT_OPERAND; 
		return op;
	}
	union
	{
		basic_operator *operatorp;
		double operand;
	}val;
	OPTOR_TYPE type;

	~optor()
	{
		if(type == OT_OPERATOR)
		{
			delete val.operatorp;
		}
	}
};


/* return operator priority */

static int priority(char opr)
{
	switch(opr)
	{
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
		case '%':
		case '^':
			return 2;
		case '(':
		case ')':
			return 3;
		default:
			return 0;
	}
	return 0;
}

/* read a double  */
static bool read_operand(string::const_iterator &beg, const string::const_iterator &end, double &operand)
{
	operand = 0;
	bool dot = false;;
	double rate = 1;
	while(beg != end)
	{
		char ch = *beg;
		if(ch >= '0' && ch <= '9')
		{
			if(dot)
			{	
				rate *= 0.1;
				operand += (ch-'0')*rate;
			}
			else
			{
				operand = operand*10 +  ch-'0';
			}
		}
		else if(ch == '.')
		{
			if(!dot)
				dot = true;
			else 
				return false;
		}
		else
		{
			break;
		}
		beg++;
	}
	return true;
}
/* read function */
static bool read_function(string::const_iterator &beg, const string::const_iterator &end, string &funcname)
{
	char ch;
	funcname.clear();
	while(beg != end)
	{
		ch = *beg;
		if(isblank(ch) || ch == '(')
		{
			break;
		}
		funcname += ch;
		beg++;
	}
	return true;
}

/* switch midfix-expression to subfix_expression */
static bool postexpr(const string &mexpr, list<optor*> &optors)
{
	deque<string> operator_stack;
	string::const_iterator it = mexpr.begin();
	double operand;
	optors.clear();
	while(it != mexpr.end())
	{
		char ch = *it;
		if((isdigit(ch)) || ch == '.' )
		{
			if(!read_operand(it, mexpr.end(), operand))
				return false;
			optor *opp = optor::create(operand);
			if(opp == NULL)
				return false;
			optors.push_back(opp);
			continue;
		}
		else if(isalpha(ch))
		{
			string funcname;
			if(!read_function(it, mexpr.end(), funcname))
				return false;
			operator_stack.push_front(funcname);
			continue;
		}
		else if(ch == ',')
		{
			while(operator_stack.size() > 0)
			{
				string tops = operator_stack.front();
				char topch = tops[0];
				if(topch == '(')
				{
					break;
				}
				else
				{
					optor *op;
					operator_stack.pop_front();
					if(isalpha(topch))
						op = optor::create(tops);
					else
						op = optor::create(topch);
					if(op == NULL)
						return false;
					optors.push_back(op);
				}
			}
		}
		else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^')
		{
			string tops;
			char topch;
			if(operator_stack.size() == 0)
			{
				operator_stack.push_front(string(1, ch));
				++it;
				continue;
			}
			tops = operator_stack.front();
			topch = tops[0];
			/* should not be a function */
			if(isalpha(topch))
				return false;
			if(topch != '(' && priority(ch) > priority(topch))
			{

				operator_stack.push_front(string(1, ch));
			}
			else
			{
				while(operator_stack.size() > 0)
				{
					string tops;
					char topch;
					tops = operator_stack.front();
					topch = tops[0];
					/* should not be a function */
					if(isalpha(topch))
						return false;
					if(topch != '(' && priority(ch) <= priority(topch))
					{
						operator_stack.pop_front();
						optor *opp =  optor::create(topch);
						if(opp == NULL)
							return false;
						optors.push_back(opp);
					}
					else
					{
						break;
					}
				}
				operator_stack.push_front(string(1, ch));
			}
		}
		else if(ch == '(')
		{
			operator_stack.push_front(string(1, ch));
		}
		else if(ch == ')')
		{	
			while(operator_stack.size() > 0)
			{	
				string tops;
				char topch;
				tops = operator_stack.front();
				operator_stack.pop_front();
				topch = tops[0];
				/* should not be a function */
				if(isalpha(topch))
					return false;
				if(topch != '(')
				{
					optor *opp = optor::create(topch);
					if(opp == NULL)
						return false;
					optors.push_back(opp);
				}
				else
				{
					tops = operator_stack.front();
					topch = tops[0];
					if(isalpha(topch))
					{
						operator_stack.pop_front();
						optor *opp = optor::create(tops);
						if(opp == NULL)
							return false;
						optors.push_back(opp);
					}
					break;
				}
			}	
		}
		/* skip blank */
		else if(!isblank(ch))
		{
			return false;
		}
		++it;
	}
	/* pop all left operator */
	while(operator_stack.size() > 0)
	{	
		string tops;
		char topch;
		optor *opp;
		tops = operator_stack.front();
		operator_stack.pop_front();
		if(tops.size() > 1)
			opp = optor::create(tops);
		else
			opp = optor::create(tops[0]);
		if(opp == NULL)
			return false;
		optors.push_back(opp);
	}	
	return true;
}

/* evaluate postfix expressions  */
static bool eval_postexpr(const list<optor*>& optors, double* result)
{
	deque<double> operand_stack;
	for(list<optor*>::const_iterator it = optors.begin(); it != optors.end(); ++it)
	{
		if((*it)->type == OT_OPERAND)
		{
			operand_stack.push_front((*it)->val.operand);
		}
		else
		{
			basic_operator *opp = (*it)->val.operatorp;
			int argnum = opp->argnum();
			if(operand_stack.size() < argnum)
			{
				assert(false);
				return false;
			}
			double d1, d2, d3, result;
			if(argnum > 0)
			{
				d1 = operand_stack.front();
				operand_stack.pop_front();
			}
			if(argnum > 1)
			{
				d2 = operand_stack.front();
				operand_stack.pop_front();
			}
			if(argnum > 2)
			{
				d3 = operand_stack.front();
				operand_stack.pop_front();
			}
			switch(argnum)
			{
				case 1:
					result = (*it)->val.operatorp->calc(d1);
					break;
				case 2:
					result = (*it)->val.operatorp->calc(d2, d1);
					break;
				case 3:
					result = (*it)->val.operatorp->calc(d3, d2, d1);
					break;
				default:
					assert(false); /* allow at most 3 arguments now */
					return false;
			}
			operand_stack.push_front(result);
		}
	}
	if(operand_stack.size() != 1)
	{
		assert(false);
		return false;
	}
	else
	{
		*result =  operand_stack.front();
	}
	return true;
}


/* evaluate midfix expression */
bool eval(const std::string& mexpr, double *result)
{
	bool ret;
	list<optor*> optors;
	if(postexpr(mexpr, optors) && eval_postexpr(optors, result))
		ret = true;
	else
		ret = false;
	/* release */
	for(list<optor*>::iterator it = optors.begin(); it != optors.end(); ++it)
	{
		delete (*it);
	}
	return ret;
}

/*
int main(int argc, char* argv[])
{
	string mexpr = "1+sqrt(sqrt(2))+3*pow(2,3)";//"1+225.34*3-(4.897+5)*(67.8+56)+78.9"
	list<optor*> optors;
	mexpr = argv[1];
	if(postexpr(mexpr, optors))
	{
		for(list<optor*>::iterator it = optors.begin(); it != optors.end(); ++it)
		{
			if((*it)->type == OT_OPERATOR)
				cout << typeid(*((*it)->val.operatorp)).name();
			else if((*it)->type == OT_OPERAND)
				cout << (*it)->val.operand;
			cout << " , ";
		}
	}
	else
	{
		cout << "false" << endl;
	}

	double result;
	if(eval(mexpr, &result))
		cout << "\n" << result << endl;
	else
		cout << "eval failed" << endl;

	return 0;
}

*/
