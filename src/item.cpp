#include "item.h"

using namespace std;

item::item(int id, const string &name, unsigned cycle, const string &expr)
	: id_ (id)
	, name_ (name)
	, cycle_ (cycle)
	, onlyexpr_ (true)
	, expr_ (expr)
	, status_ (false) 
{
	
}

item::~item()
{
}
