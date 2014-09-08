#ifndef _COMM_SERIALPORT_LINUX_H_
#define _COMM_SERIALPORT_LINUX_H_

#include "comm_serialport.h"

class comm_serialport_linux : public comm_serialport
{
public:
	comm_serialport_linux(uint16 com);
	~comm_serialport_linux(void);
	virtual bool setoption(uint32 baudrate, uint8 databits, uint8 stopbits, uint8 flowctrl, uint8 parity);
	virtual bool clear();
	virtual bool open();
	virtual void close();

	// write size, return until size bytes been written or timeout
	virtual size_t write(const void *data, size_t size);
	// read size, return until size bytes been read or timeout
	virtual size_t read(void *data, size_t size);

protected:
	int fd_;
};


#endif
