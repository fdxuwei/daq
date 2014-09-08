#ifndef _COMM_SERIALPORT_H_
#define _COMM_SERIALPORT_H_

#include <string>
#include "comm.h"
#include "daqtypes.h"

class comm_serialport : public comm
{
public:

	virtual bool setoption(uint32 baudrate, uint8 databits, uint8 stopbits, uint8 flowctrl, uint8 parity) = 0;
	virtual bool clear() = 0;
	virtual bool open() = 0;
	virtual void close() = 0;

	// write size bytes, return until size bytes been written or timeout
	virtual size_t write(const void *data, size_t size) = 0;
	// read size bytes, return until size bytes been read or timeout
	virtual size_t read(void *data, size_t size) = 0;

protected:
	uint32 baudrate_;
	uint8 bytesize_;
	uint8 parity_;
	uint8 stopbits_;
	uint8 flowctrl_;
};

#endif
