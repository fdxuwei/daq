#ifndef _ACQER_MODBUS_SERIALPORT_H_
#define _ACQER_MODBUS_SERIALPORT_H_

#include "acqer_modbus.h"

class comm;

class acqer_modbus_serialport: public acqer_modbus
{
public:
	acqer_modbus_serialport(uint16 port, uint16 baudrate, uint8 databits, uint8 stopbits, uint8 flowctl, uint8 parity);
protected:
	virtual comm* init_comm();
private:
	uint16 port_;
	uint16 baudrate_;
	uint8 databits_;
	uint8 stopbits_;
	uint8 flowctl_;
	uint8 parity_;
};

#endif

