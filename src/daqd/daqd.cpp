#include <string>
#include <sys/epoll.h>
#include "macros.h"
#include "system.h"
#include "Markup.h"

using namespace std;

/* run */
bool daqd_run = 1;

/* gateway */
gateway *gateway1 = NULL;

/* epoll */
int epollfd;

/* modules */
std::vector<module*> modules;
std::map<std::string, module*> moduleofname;
std::map<int, module*> moduleoffd;

/* signal handlers */
static void sig_int(int signo)
{
	daqd_run = 0;
}

static void sig_child(int signo)
{

}


/* main routin */
int main()
{
	CMarkup cfg;
	/**/
	epollfd = epoll_create(10);
	if(epollfd < 0)
		err_exit("epoll_create");
	/**/
	if(!cfg.Load(DAQD_CINFIG_FILE))
		err_exit("Load daqd config");
	/* gateway */
	if(cfg.FindElem("gateway"))
	{
		string ip;
		unsigned port;
		if(!cfg.HasAttrib("ip"))
			err_exit("gateway has no ip");
		ip = cfg.GetAttrib("ip");
		if(!cfg.HasAttrib("port"))
			err_exit("gateway has no port");
		port = str2int(cfg.GetAttrib("port"));
		gateway1 = new gateway(ip, port);
/*		if((fd = gateway1->connect()) < 0)
			err_exit("connect %s:%u failed", ip.c_str(), port);
		ev.data.fd = fd;
		ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
		if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) < 0)
		err_exit("epoll_ctl");
*/
	}

	/* acq module */
	if(cfg.FindElem("modules"))
	{
		string mod_name;
		int mod_run;
		cfg.IntoElem();
		while(cfg.FindElem("module"))
		{
			if(!cfg.HasAttrib("name"))
				err_exit("module has no name");
			mod_name = cfg.GetAttrib("name");
			if(!cfg.HasAttrib("run"))
				err_exit("module has no run");
			mod_run = str2int(cfg.GetAttrib("run"));
			if(mod_run)
			{
				module *pm = new module(mod_name);
				modules.push_back(pm);
			}
		}
		cfg.OutOfElem();
	}

	int fd;
	struct epoll_event ev;
	/* run gateway */
	if(NULL != gateway1)
	{
		if((fd = gateway1->connect()) < 0)
			err_exit("connect %s:%u failed", ip.c_str(), port);
		ev.data.fd = fd;
		ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
		if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) < 0)
			err_exit("epoll_ctl");
	}
	/* run modules */
	for(list<module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		if(!(*it)->run())
			;
	}
	
	/* main loop */
	struct epoll_event events[EPOLL_MAX_EVENTS];
	int numevents;
	while(daqd_run && 
			(numevents = epoll_wait(epollfd, events, EPOLL_MAXEVENTS, EPOLL_WAIT_TIMEOUT)) >= 0)
	{
		for(int i = 0; i < numevents; ++i)
		{
			int fd = events[i].data.fd;
			if(fd == gateway1->fd())
			{
/*				if(EPOLLERR & events[i].events)
					gateway1->handle_err();
				if(EPOLLHUP & events[i].events)
					gateway1->handle_hup();
				if(EPOLLIN & events[i].events)
					gateway1->handle_read();
				if(EPOLLOUT & events[i].events)
					gateway1->handle_write();
*/
			}
			else
			{
				if(EPOLLERR & events[i].events)
					moduleoffd[fd]->handle_err();
				if(EPOLLHUP & events[i].events)
					moduleoffd[fd]->handle_hup();
				if(EPOLLIN & events[i].events)
					moduleoffd[fd]->handle_read();
				if(EPOLLOUT & events[i].events)
					moduleoffd[fd]->handle_write();
			}
		}
	}

	while(wait(NULL) != ECHILD);

	if(numevents < 0)
		err_exit("epoll_wait");
	return 0;
}

