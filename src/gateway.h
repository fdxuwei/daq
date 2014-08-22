#ifndef _GATEWAY_H_
#define _GATEWAY_H_

#include <string>

class gateway
{
public:
	gateway(const std::string &ip, unsigned port)
	~gateway();
	int connect();
	int fd();
private:
	std::string ip_;
	unsigned port_;
	int fd_;
};

#endif

