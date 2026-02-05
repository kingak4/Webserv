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

using namespace std;

int main()
{
	int epoll_fd = epoll_create1(0);
	struct epoll_event event, events[10];
	event.events = EPOLLIN | EPOLLOUT | EPOLLET;

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(serverSocket);
		throw std::runtime_error("setsockopt error(SO_REUSEADDR)");
	}

	/*int status = fcntl(serverSocket, F_SETFL, fcntl(serverSocket, F_GETFL, 0) | O_NONBLOCK);
	if (status == -1)
		std::cerr << "fcntl error\n";
	*/
	event.data.fd = serverSocket;

    // binding socket.
    if (bind(serverSocket, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress)) == -1)
	{
		close(serverSocket);
		close(epoll_fd);
		throw std::runtime_error("binding error");
	}
	// listening to the assigned socket
	listen(serverSocket, 5);

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket, &event))
	{
		throw std::runtime_error("epoll_ctl: ADD");
	}
	
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
    close(serverSocket);

    return 0;
}
