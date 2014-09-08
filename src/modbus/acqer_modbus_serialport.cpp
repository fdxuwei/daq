#include "acqer_modbus_serialport.h"
#include "serialport_pool.h"
#include "comm_serialport.h"
#include "ProtoModbus.h"

using namespace std;

acqer_modbus_serialport::acqer_modbus_serialport(uint8 devaddr, uint16 port, uint16 baudrate, uint8 databits, uint8 stopbits, uint8 flowctl, uint8 parity)
	: acqer_modbus(devaddr)
	, port_ (port)
	, baudrate_ (baudrate)
	, databits_ (databits)
	, stopbits_ (stopbits)
	, flowctl_ (flowctl)
	, parity_ (parity)
{
	protomod_ = new CProtoModbusCom;
}

comm* acqer_modbus_serialport::init_comm()
{
	comm_serialport * cs = serialport_pool::instance().get_serialport(port_);
	
	/* can be optimized to reduce open/close times */
	if(cs->isopen())
		cs->close();
	if(!cs->open())
	{
		return NULL;
	}
	if(!cs->setoption(baudrate_, databits_, stopbits_, flowctl_, parity_))
	{
		return NULL;
	}

	return cs;
}


