#include "Server.hpp"

using namespace std;


//Constructors
Server::Server(int port) :  port(port) 
{  
	this->server_init();
}
Server::~Server(void) {  }


//Member getters
int Server::get_port(void) const {return port;}
int Server::get_socket(void) const {return socket_fd;}
int Server::get_epoll_fd(void) const {return epoll_fd;}
struct epoll_event Server::get_server_event(void) {return event;}
struct epoll_event Server::get_active_event(int index) {return active_events[index];} 


//Helper functions
bool accept_connection(Server &serv)
{
	int clientSocket = accept(serv.get_socket(), NULL, NULL);
	if (clientSocket == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false;
		return false;
	}
	cout << "New Client connected." << endl;
	int status = fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	if (status == -1)
		cerr << "fcntl error\n";
	struct epoll_event event = serv.get_server_event();
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = clientSocket;
	epoll_ctl(serv.get_epoll_fd(), EPOLL_CTL_ADD, clientSocket, &event);
	return true;
}

string process_request(string &request_str)
{
	//pass request to request parser
	(void)request_str; // Silences the "unused parameter" error
    return "HTTP/1.1 200 OK\r\n\r\nHello World";
}

void respond_to_client(struct epoll_event client, string &response_str)
{
	/*2. Before write I should change flag of the socket
	 *and then write to the client socket*/

	write(client.data.fd, response_str.c_str(), response_str.length());
	
	close(client.data.fd);
}


//Memeber functions
void Server::server_init(void)
{
	this->epoll_fd = epoll_create(10);

	this->event.events = EPOLLIN | EPOLLOUT | EPOLLET;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    this->socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(socket_fd);
		throw runtime_error("setsockopt error(SO_REUSEADDR)");
	}

	this->event.data.fd = socket_fd;

    if (bind(socket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		close(socket_fd);
		close(epoll_fd);
		throw runtime_error("binding error");
	}

	listen(socket_fd, 5);

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event))
	{
		throw runtime_error("epoll_ctl ADD");
	}
}

void Server::server_loop(void)
{
	while (true)
	{
		int num_fds = epoll_wait(epoll_fd, active_events, 10, -1);
		for (int i = 0; i < num_fds; i++)
		{
			if (active_events[i].data.fd == socket_fd)
			{
				while(true)
					if (!accept_connection(*this))
						break;
			}
			else
			{
				char buffer[120] = { 0 };
				stringstream entire_request;
				while (true)
				{
					ssize_t count = read(active_events[i].data.fd, buffer, sizeof(buffer));
					if (count == -1)
					{
						string request_content = entire_request.str();
						string response_str = process_request(request_content);
						if (!response_str.empty())
							respond_to_client(active_events[i], response_str);
						else
							throw runtime_error("empty response");
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
						cout << "Client: " << i << " disconnected." << endl;
						close(active_events[i].data.fd);
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
    close(socket_fd);
}

int main()
{
	Server server(8080);

	try
	{
		server.server_init();
		server.server_loop();
	}
	catch (runtime_error &e)
	{
		cout << RED << "ERROR: " << e.what() << RESET << endl;
	}
	return 0;
}
