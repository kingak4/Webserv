#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../include/http/Parser.hpp"
#include "Client.hpp"

#include <netinet/in.h>
#include <fcntl.h>

class EpollManager;

static const std::string RED = "\033[0;31m";
static const std::string RESET = "\033[0m";
static const std::string GREEN = "\033[0;32m";
static const std::string BLUE = "\033[0;36m";
static const std::string BOLD_BLUE = "\033[1;36m";

class Server
{
	private:
		int port;
		int socket_fd;
		EpollManager &epoll_manager;
	public:
		Server(int port, EpollManager &manager);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server(void);
		int get_port(void) const;
		int get_socket(void) const;
		EpollManager &get_Epoll_Manager(void) const;
		void server_init(void);
};
#endif
