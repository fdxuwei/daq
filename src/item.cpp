#include "item.h"

using namespace std;

item::item(int id, const string &name, unsigned cycle, const string &expr, ITEM_VALUE_TYPE ivt)
	: id_ (id)
	, name_ (name)
	, cycle_ (cycle)
	, onlyexpr_ (true)
	, expr_ (expr)
	, status_ (IS_OLD) 
{
	value_.ivt_ = ivt;
}

item::~item()
{
}
