#ifndef _SERIALPORT_POOL_H_
#define _SERIALPORT_POOL_H_

#include <map>
#include <list>

class comm_serialport;

class serialport_pool
{
public:
	static serialport_pool &instance()
	{
		static serialport_pool sp;
		return sp;
	}
	void add_serialport(uint16 port);
	comm_serialport* get_serialport(uint16 port);
	~serialport_pool();
private:	
	seralport_pool();
	std::list<comm_serialport*> sps_; // all SerialPort
	std::map<uint16, comm_serialport*> port_sp_; // port - SerialPort* map
};


#endif

