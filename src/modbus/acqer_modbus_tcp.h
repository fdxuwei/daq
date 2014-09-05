#ifndef _ACQER_MODBUS_TCP_H
#define _ACQER_MODBUS_TCP_H

#include "acqer_modbus.h"

class acqer_modbus_tcp : public acqer_modbus
{
public:
private:
	std::string ip_;
	uint16 port_;
};

#endif

