#include "../../include/core/EpollManager.hpp"
#include <iterator>

// sig_atomic_t: an integer type that can be accessed atomically
volatile sig_atomic_t g_server_running = 1;

void signal_handler(int signum) {
    (void)signum; // avoid unused parameter warning
    g_server_running = 0; 
}

EpollManager::EpollManager(void) {  }

EpollManager::~EpollManager(void)
{
	map<int, Server*>::iterator it;
	for (it = this->servers_running.begin(); it != this->servers_running.end(); ++it)
	{
		cout << "destructor ERROR\n";
		delete it->second;
	}
	this->servers_running.clear();
	close(this->epoll_fd);
	cout << "Shutting server " << RED << "OFF" << RESET << endl;
}

int EpollManager::get_Epoll_Fd(void)
{
	return (this->epoll_fd);
}
const struct epoll_event &EpollManager::get_Epoll_Event(void) const
{
	return (this->event);
}
const map<int, Server*> &EpollManager::get_Servers_Running(void) const
{
	return (this->servers_running);
}

void EpollManager::init_Epoll(vector<ServerData> &config_splitted)
{
	this->epoll_fd = epoll_create(10);
	this->event.events = EPOLLIN | EPOLLOUT;

	for (vector<ServerData>::iterator it = config_splitted.begin(); it != config_splitted.end(); ++it)
	{
		Server *server = new Server(it->port, *this);		
		int socket_fd = server->get_socket();

		this->event.data.fd = socket_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1)
			throw runtime_error("epoll_ctl ADD");
		this->servers_running.insert(make_pair(socket_fd, server));
	}

/*	for (int i = 8080; i < 8083; ++i)
	{
		Server *server = new Server(i, *this);		
		int socket_fd = server->get_socket();

		this->event.data.fd = socket_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1)
			throw runtime_error("epoll_ctl ADD");
		this->servers_running.insert(make_pair(socket_fd, server));
	}
	*/

}

Client *accept_connection(Server *serv)
{
	int clientSocket = accept(serv->get_socket(), NULL, NULL);
	Client *client = new Client(clientSocket, serv);
	
	EpollManager &epoll_manager = serv->get_Epoll_Manager();
	if (clientSocket != -1)
	{
		cout << "New Client connected." << endl;
		int status = fcntl(clientSocket, F_SETFL, O_NONBLOCK);
		if (status == -1)
		{
			close(clientSocket);
			throw runtime_error("fcntl error");
		}
		struct epoll_event event; //= epoll_manager.get_Epoll_Event();
		event.events = EPOLLIN | EPOLLOUT;
		event.data.fd = clientSocket;
		epoll_ctl(epoll_manager.get_Epoll_Fd(), EPOLL_CTL_ADD, clientSocket, &event);
	}
	return client;
}

string process_request(const string &request_str)
{
	Parser parser(request_str);
	parser.parse_Request();

	cout << (parser.is_Valid() ? GREEN : RED) << "Is valid: " << (parser.is_Valid() ? "TRUE " : "FALSE ") << endl;
	cout << (parser.is_Valid() ? GREEN : RED) << "Method: " << parser.get_Method() << endl;
	cout << (parser.is_Valid() ? GREEN : RED) << "Path: " << parser.get_Path() << endl;

	std::map<std::string, std::string> headers = parser.get_Headers();
	std::map<string, string>::iterator it;
	cout << (parser.is_Valid() ? GREEN : RED) << "Headers: " << endl;
	for (it = headers.begin(); it != headers.end(); ++it)
	{
		cout << (parser.is_Valid() ? GREEN : RED) << it->first << " : " << it->second << endl;
	}
	cout << RESET << endl;

    return "HTTP/1.1 200 OK\r\n\r\nHello World";
}

void handle_Read(struct epoll_event &event, Client &client)
{
	char buffer[1024];
	
	while (true)
	{
		ssize_t count = read(event.data.fd, buffer, sizeof(buffer));		
		if (count == -1)
		{
			//string request_content = entire_request.str();
			client.set_Response(process_request(client.get_Buffer()));
			stringstream portStr;
			portStr << endl << "Port:" << client.get_Server()->get_port();
			client.set_Response(client.get_Response() + portStr.str());
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
			cout << "Client: " << event.data.fd << " disconnected." << endl;
			close(event.data.fd);
			break;
		}
		else
		{
			//entire_request << buffer;
			client.append_Buffer(buffer, count);
		}
	}
}

bool handle_Write(struct epoll_event &event, Client &client)
{
	string response = client.get_Response();

	if (write(event.data.fd, response.c_str(), response.length()) != (ssize_t)response.length())
		cout << "Invalid write" << endl;
	return true;
}

void EpollManager::epoll_Loop(void)
{
	map<int, Client*> client_map;
	while (g_server_running)
	{
		int num_fds = epoll_wait(epoll_fd, active_events, 10, -1);
		if (num_fds == -1)
		{
			if (errno == EINTR)
				continue ;
			else 
				throw runtime_error("epoll_wait error");
		}
		for (int i = 0; i < num_fds; i++)
		{
			int fd = active_events[i].data.fd;
			if (active_events[i].events & (EPOLLERR | EPOLLHUP))
			{
				if (this->servers_running.count(fd)) 
				{
					std::cerr << "Listener error on fd " << fd << std::endl;
					continue;
				}
				map<int, Client*>::iterator it = client_map.find(fd);
				if (it != client_map.end())
				{
					cout << "EPOLLERR | ERROR\n";
					delete it->second;
					client_map.erase(it);
				}
				close(fd);
				continue;
			}
			if (active_events[i].events & EPOLLIN)
			{
				if (this->servers_running.count(fd))
				{
					Client *client = accept_connection(this->servers_running.at(fd));
					//client_map[client.get_Socket()] = client;
					client_map.insert(make_pair(client->get_Socket(), client));
				}
				else 
					handle_Read(active_events[i], *client_map[fd]);
			}
			if (active_events[i].events & EPOLLOUT)
			{
				if (client_map.find(fd)->second->get_Response().length() <= 0)
					continue;
				std::map<int, Client*>::iterator it = client_map.find(fd);
				if (it != client_map.end())
				{
					if (handle_Write(active_events[i], *it->second))
					{
						epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, fd, NULL);
						close(fd);
						cout << "ERROR\n";
						delete it->second;
						client_map.erase(it);
					}
				}
				//handle_Write(active_events[i], client_map[fd]);
			}
			//	handle_Write(active_events[i], client_map.find(fd));
		}
	}
	//close_epoll()
	map<int, Client*>::iterator it;
	for (it = client_map.begin(); it != client_map.end(); ++it)
	{
		close(it->first);
		cout << "ERROR 2\n";
		delete it->second;
	}
	client_map.clear();
}

