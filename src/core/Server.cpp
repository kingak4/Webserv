#include "../../include/core/Server.hpp"
#include "../../include/core/EpollManager.hpp"
using namespace std;

//Constructors
Server::Server(int port, EpollManager &manager) :  port(port), epoll_manager(manager)
{  
	this->server_init();
}
Server::~Server(void) 
{
	stringstream ss;
	ss << "Closing port " << this->port << ".";  
	Console::message(ss.str(), INFO, false);
}

Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->port = other.get_port();
		this->socket_fd = other.get_socket();
	}
	return *this;
}
Server::Server(const Server &other) : port(other.get_port()), epoll_manager(other.get_Epoll_Manager())
{
	this->port = other.get_port();
	this->socket_fd = other.get_socket();
}

//Member getters
int Server::get_port(void) const {return port;}
int Server::get_socket(void) const {return socket_fd;}
EpollManager &Server::get_Epoll_Manager(void) const {return epoll_manager;}

//Memeber functions
void Server::server_init(void)
{
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    this->socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

	{
	stringstream ss;
	ss << "Port " << this->port << " socket created succesfuly.";
	Console::message(ss.str(), INFO, false);
	}

	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(socket_fd);
		throw runtime_error("setsockopt error(SO_REUSEADDR)");
	}

    if (bind(socket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		close(socket_fd);
		throw runtime_error("binding error");
	}

	{
	stringstream ss;
	ss << "Port " << this->port << " socket bind() succesful.";
	Console::message(ss.str(), INFO, false);
	}

	if (listen(socket_fd, 5) == -1)
	{
		stringstream ss;
		ss << "listen() error on port " << port <<  ". error code: " << errno;
		throw runtime_error(ss.str());
	}

	{
	stringstream ss;
	ss << "Port " << this->port << " listen() succesful.";
	Console::message(ss.str(), INFO, false);
	}
}
