#ifndef EPOLLMANAGER_HPP
#define EPOLLMANAGER_HPP

#include <iostream>
#include <sys/epoll.h>
#include <map>
#include "Server.hpp"
#include "../../include/config/ConfigParser.hpp"
#include "Client.hpp"
#include <csignal>

using namespace std;

void signal_handler(int signum);

class EpollManager
{
	private:
		int epoll_fd;
		struct epoll_event event, active_events[10];
		map<int, Server*> servers_running;
	public:
		EpollManager(void);
		~EpollManager(void);
		void init_Epoll(vector<ServerData> &config_splitted);
		void epoll_Loop(void);
		int get_Epoll_Fd(void);
		const struct epoll_event &get_Epoll_Event(void) const;
		const map<int, Server *> &get_Servers_Running(void) const;
};

#endif 
