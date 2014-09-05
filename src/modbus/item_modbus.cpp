#include "item_modbus.h"

using namespace std;

item_modbus::item_modbus(int id, const string &name, unsigned cycle, const string &expr, bool onlyexpr, uint8 funcode, uint16 startaddr, uint8 sa_offser, uint16 count)
	: item(id, name, cycle, expr)
	, onlyexpr_ (onlyexpr)
	, funcode_ (funcode)
	, startaddr_ (startaddr)
	, sa_offset _ (sa_offset)
	, count_ (count)
{}

