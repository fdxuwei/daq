#include <string>
#include <sys/epoll.h>
#include "macros.h"
#include "system.h"
#include "Markup.h"

using namespace std;

int gateway_fd;
string gateway_ip;
unsigned gateway_port;

/* epoll */
int epollfd;
struct epoll_event events[EPOLL_MAX_EVENTS];

int main()
{
	CMarkup cfg;
	struct epoll_event ev;
	/**/
	epollfd = epoll_create(10);
	if(epollfd < 0)
		err_sys("epoll_create");
	/**/
	if(!cfg.Load(DAQD_CINFIG_FILE))
		err_sys("Load daqd config");
	if(cfg.FindElem("gateway"))
	{
		if(!cfg.HasAttrib("ip"))
			err_sys("gateway has no ip");
		gateway_ip = cfg.GetAttrib("ip");
		if(!cfg.HasAttrib("port"))
			err_sys("gateway has no port");
		gateway_port = str2int(cfg.GetAttrib("port"));
		if((gateway_fd = connect_tcp(gateway_ip, gateway_port)) < 0)
			err_sys("connect %s:%u failed", gateway_ip.c_str(), gateway_port);
		ev.data.fd = gateway_fd;
		ev.events = EPOLLIN | EPOLLOUT | EPOLLET
	}
	return 0;
}

