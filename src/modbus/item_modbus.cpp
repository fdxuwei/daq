#include "item_modbus.h"

using namespace std;

item_modbus::item_modbus(int id, const string &name, unsigned cycle, const string &expr, ITEM_VALUE_TYPE ivt, bool onlyexpr, uint8 funcode, uint16 startaddr, uint8 sa_offset, uint16 count)
	: item(id, name, cycle, expr, ivt)
	, funcode_ (funcode)
	, startaddr_ (startaddr)
	, sa_offset_ (sa_offset)
	, count_ (count)
{
	onlyexpr_ = onlyexpr;
}

