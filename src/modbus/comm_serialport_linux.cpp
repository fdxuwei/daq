#include "comm_serialport_linux.h"
#include <sstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

using namespace std;


comm_serialport_linux::comm_serialport_linux(uint16 com)
	: fd_ (-1)
{
	stringstream ss;
	ss << "/dev/ttyS" << com-1;
	name_ = ss.str();
	timeout_ = 2;
}

comm_serialport_linux::~comm_serialport_linux(void)
{
	close();
}


bool comm_serialport_linux::setoption(uint32 baudrate, uint8 databits, uint8 stopbits, uint8 flowctl, uint8 parity) 
{
	static map<uint32, uint16> baudmap;
	static map<uint8, uint8> databitsmap;
	static bool first = true;
	if(first)
	{
		first = false;
		baudmap[0] = B0;
		baudmap[50] = B50;
		baudmap[75] = B75;
		baudmap[110] = B110;
		baudmap[134] = B134;
		baudmap[150] = B150;
		baudmap[200] = B200;
		baudmap[300] = B300;
		baudmap[600] = B600;
		baudmap[1200] = B1200;
		baudmap[1800] = B1800;
		baudmap[2400] = B2400;
		baudmap[4800] = B4800;
		baudmap[9600] = B9600;
		baudmap[19200] = B19200;
		baudmap[38400] = B38400;
		baudmap[57600] = B57600;
		baudmap[115200] = B115200;
		baudmap[230400] = B230400;
	
		databitsmap[5] = CS5;
		databitsmap[6] = CS6;
		databitsmap[7] = CS7;
		databitsmap[8] = CS8;
	}

	if(fd_ < 0)
	{
		return false;
	}
	struct termios tio;
	memset(&tio, 0 , sizeof(tio));
	if(tcgetattr(fd_, &tio) < 0)
	{
		return false;
	}
	cfsetispeed(&tio, baudmap[baudrate]);
	cfsetospeed(&tio, baudmap[baudrate]);
	/* enable the receiver and set local mode .. */
	tio.c_cflag |= (CLOCAL | CREAD);
	/* set data bits */
	tio.c_cflag &= ~CSIZE;
	tio.c_cflag |= databitsmap[databits];
	/* set parity */
	switch(parity)
	{
		case 0: /* no parity */
			tio.c_cflag &= ~PARENB;
			break;
		case 1: /* odd parity */
			tio.c_cflag |= PARENB;
			tio.c_cflag |= PARODD;
			break;
		case 2: /* even parity */
			tio.c_cflag |= PARENB;
			tio.c_cflag &= ~PARODD;
			break;
		case 3: /* space parity */
			tio.c_cflag &= ~PARENB;
		default: /* not possible */ 
			assert(false);
	}
	/* set stopbits  */
	switch(stopbits)
	{
		case 1: /* one stop bits */
			tio.c_cflag &= ~CSTOPB;
			break;
		case 2: /* two stop bits */
			tio.c_cflag |= CSTOPB;
			break;
		default: /* not possible */
			assert(false);
	}
	/* set flowctl */
	switch(flowctl)
	{
		case 0: /*no flowctl*/
			tio.c_cflag &= ~CRTSCTS;
			break;
		case 1: /* hard ctl */
			tio.c_cflag |= CRTSCTS;
			break;
		case 2: /* soft ctl */
			tio.c_iflag |= IXON | IXOFF | IXANY;
			break;
		default: /* not possible */
			assert(false);
	}
	// raw mode
	tio.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	tio.c_oflag &= ~OPOST;
	//	
	tio.c_cc[VTIME] = timeout_*10;
	tio.c_cc[VMIN] = 0;
	if(tcsetattr(fd_, TCSANOW, &tio) < 0)
	{
		return false;
	}
	return true;
}


bool comm_serialport_linux::open()
{
	if(!isopen())
	{
		close();
		fd_ = ::open(name_.c_str(), O_RDWR);
		if(fd_ < 0)
			return false;
		isopen_ = true;
	}
	return true;
}

void comm_serialport_linux::close()
{
	if(fd_ > 0)
	{
		::close(fd_);
		fd_ = -1;
	}
	isopen_ = false;
}	


size_t comm_serialport_linux::write(const void *data, size_t size)
{
	int once_len  = 0;
	size_t wsize = 0;
	while(wsize < size)
	{
		if((once_len = ::write(fd_, (char*)data+wsize, size-wsize)) < 0)
		{
			break;
		}
		if(0 == once_len) // timeout
		{
			break;
		}
		wsize += once_len;
	}
	return wsize;
}

size_t comm_serialport_linux::read(void *data, size_t size)
{
	int once_len = 0;
	size_t rsize = 0;
	while(rsize < size)
	{
		if((once_len = ::read(fd_, (char*)data+rsize, size-rsize)) < 0)
		{
			break;;
		}
		if(0 == once_len) // timeout
		{
			break;
		}
		rsize += once_len;
	}

	return rsize;
}

bool comm_serialport_linux::clear()
{
	return (tcflush(fd_, TCIOFLUSH) == 0);  
}
