#ifndef EPOLLMANAGER_HPP
#define EPOLLMANAGER_HPP

#include <sys/epoll.h>
#include "Server.hpp"
#include "../../include/config/ConfigParser.hpp"
#include "../../include/config/Config.hpp"
#include "../../include/config/Route.hpp"
#include "../../include/http/Request.hpp"
#include "Client.hpp"
#include <csignal>

using namespace std;

enum Message_type
{
	INFO,
	NOTICE,
	WARNING,
	ERROR,
	SUCCES,
	GET,
	POST,
	DELETE
};

namespace Console
{
	void message(const string &message, Message_type type, bool pre_newline);
}

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
		void epoll_Loop(ConfigParser &config_parser);
		int get_Epoll_Fd(void);
		const struct epoll_event &get_Epoll_Event(void) const;
		const map<int, Server *> &get_Servers_Running(void) const;
};

#endif 
