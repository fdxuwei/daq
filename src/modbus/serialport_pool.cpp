#include "serialport_pool.h"
#include "comm_serialport_linux.h"

using namespace std;

serialport_pool::serialport_pool()
{}

serialport_pool::~serialport_pool()
{
	/* release comm_serialport */
	for(list<comm_serialport*>::iterator it = sps_.begin(); it != sps_.end(); ++it)
	{
		delete (*it);
	}
}

void serialport_pool::add_serialport(uint16 port)
{
	/* already exist  */
	if(port_sp_.find(port) != port_sp_.end())
		return;
		
	comm_serialport *spp = NULL;
#ifndef WIN32	
	spp = new comm_serialport_linux(port);
#else
	/* not supported */
	assert(false);
	/* spp = new comm_serialport_win(port); */
#endif	
	sps_.push_back(spp);
	port_sp_[port] = spp;
}

comm_serialport* serialport_pool::get_serialport(uint16 port)
{
	std::map<uint16, comm_serialport*>::iterator it = port_sp_.find(port);
	if(it == port_sp_.end())
		return NULL;
	else
		return *(it->second);
}


