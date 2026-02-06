#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
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
		int const get_port(void);
		int const get_socket(void);
		int const get_epoll_fd(void);
		struct epoll_event get_server_event(void);
		struct epoll_event get_active_event(int index);
		void server_init(void);
};
#endif
