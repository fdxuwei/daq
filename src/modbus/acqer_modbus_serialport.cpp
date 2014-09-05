#include "acqer_modbus_serialport.h"
#include "serialport_pool.h"
#include "comm_serialport.h"

using namespace std;

comm* acqer_modbus_serialport::init_comm()
{
	comm_serialport * cs = serialport_pool.get_serialport(port_);
	
	/* can be optimized to reduce open/close times */
	if(isopen())
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


