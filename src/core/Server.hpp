#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>


static const std::string RED = "\033[0;31m";
static const std::string RESET = "\033[0m";
static const std::string GREEN = "\033[0;32m";

class Server
{
	private:
		int port;
		int socket_fd;
		int epoll_fd;
		struct epoll_event event, active_events[10];
	public:
		Server(int port);
		~Server(void);
		int get_port(void) const;
		int get_socket(void) const;
		int get_epoll_fd(void) const;
		struct epoll_event get_server_event(void);
		struct epoll_event get_active_event(int index);
		void server_init(void);
		void server_loop(void);
};
#endif
