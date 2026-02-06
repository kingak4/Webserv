// C++ program to show the example of server application in
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <cerrno>
#include "Server.hpp"

using namespace std;


Server::Server(int port) : port(port) {  }
Server::~Server(void) {  }

int const Server::get_socket(void) {return socket_fd;}
int const Server::get_epoll_fd(void) {return epoll_fd;}
struct epoll_event Server::get_server_event(void) {return event;}
struct epoll_event Server::get_active_event(int index) {return active_events[index];} 

void Server::server_init(void)
{
	int server_epoll_fd = epoll_create(10);
	this->epoll_fd = server_epoll_fd;

	this->event.events = EPOLLIN | EPOLLOUT | EPOLLET;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->get_port());
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int serv_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	this->socket_fd = serv_socket;

	int opt = 1;
	if (setsockopt(serv_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(serv_socket);
		throw std::runtime_error("setsockopt error(SO_REUSEADDR)");
	}

	this->event.data.fd = serv_socket;

    if (bind(serv_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		close(serv_socket);
		close(server_epoll_fd);
		throw std::runtime_error("binding error");
	}

	listen(serv_socket, 5);

	if (epoll_ctl(server_epoll_fd, EPOLL_CTL_ADD, serv_socket, &event))
	{
		std::stringstream ss;
		ss << errno;
		throw std::runtime_error(ss.str() + ": epoll_ctl: ADD");
	}
}

int main()
{

    // specifying the address
    // creating socket


    // binding socket.
	// listening to the assigned socket
	
	while (true)
	{
		int num_fds = epoll_wait(epoll_fd, events, 10, -1);
		for (int i = 0; i < num_fds; i++)
		{
			if (events[i].data.fd == serverSocket)
			{
				while(true)
				{
					// accepting connection request
					int clientSocket
						= accept(serverSocket, NULL, NULL);
					if (clientSocket == -1)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
							break;
						break;
					}
					std::cout << "New Client connected." << std::endl;
					//set_nonblocking(clientSocket);
					int status = fcntl(clientSocket, F_SETFL, fcntl(clientSocket, F_GETFL, 0) | O_NONBLOCK);
					if (status == -1)
						std::cerr << "fcntl error\n";
					event.events = EPOLLIN | EPOLLET;
					event.data.fd = clientSocket;
					epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &event);
				}
			}
			else
			{
				char buffer[120] = { 0 };
				std::stringstream entire_request;
				while (true)
				{
					ssize_t count = read(events[i].data.fd, buffer, sizeof(buffer));
					if (count == -1)
					{
						/* 1. call REQUEST_PARSER with entire_request.str() as parameter
						 *it should return string to respond to the client.*/

						/*2. Before write I should change flag of the socket
						 *and then write to the client socket*/

						//write(events[i].data.fd, respond.c_str(), respond.length());
						
						//close(events[i].data.fd);
						break;
					}
					else if (count == 0)
					{
						std::cout << "Client: " << i << " disconnected." << std::endl;
						close(events[i].data.fd);
						break;
					}
					else
					{
						entire_request << buffer;
					}
				}
			}
		}
	}

    // closing the socket.
	close(epoll_fd);
    close(serverSocket);

    return 0;
}
