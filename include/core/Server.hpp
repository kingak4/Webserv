#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../include/http/Parser.hpp"
#include "Client.hpp"

#include <netinet/in.h>
#include <fcntl.h>

class EpollManager;

static const std::string COL_INFO = "\033[0;37m";
static const std::string COL_NOTICE = "\033[1;36m";
static const std::string COL_SUCCES = "\033[1;32m";
static const std::string COL_WARNING = "\033[1;33m";
static const std::string COL_ERROR = "\033[1;31m";

static const std::string COL_GET = "\033[0;34m";
static const std::string COL_POST = "\033[0;34m";
static const std::string COL_DELETE = "\033[0;31m";
static const std::string RESET = "\033[0m";

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
