#include "../../include/core/EpollManager.hpp"

// sig_atomic_t: an integer type that can be accessed atomically
volatile sig_atomic_t g_server_running = 1;

string get_Current_Date_RFC(bool is_short)
{
	time_t current_time = time(NULL);
	struct tm *time_struct = gmtime(&current_time);
	if (!time_struct)
		return (std::string());
	const char *days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
							  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	char buffer[100];
	if (is_short)
	{
		sprintf(buffer, "%02d %s %04d, %02d:%02d:%02d GMT",
				time_struct->tm_mday,
				months[time_struct->tm_mon],
				time_struct->tm_year + 1900,
				time_struct->tm_hour,
				time_struct->tm_min,
				time_struct->tm_sec);
	}
	else
	{
		sprintf(buffer, "%s, %02d %s %04d %02d:%02d:%02d GMT",
				days[time_struct->tm_wday],
				time_struct->tm_mday,
				months[time_struct->tm_mon],
				time_struct->tm_year + 1900,
				time_struct->tm_hour,
				time_struct->tm_min,
				time_struct->tm_sec);
	}
	return (std::string(buffer));
}

void Console::message(const string &message, Message_type type, bool pre_newline)
{
	stringstream ss;
	if (pre_newline)
		cout << endl;
	ss << "[" << get_Current_Date_RFC(true) << "] ";
	switch (type)	
	{
		case INFO:
			ss << COL_INFO << "[INFO] " << message << RESET << endl;
			break;
		case NOTICE:
			ss << COL_NOTICE  << "[NOTICE] "<< message << RESET << endl;
			break;
		case WARNING:
			ss << COL_WARNING << "[WARNING] "<< message << RESET << endl;
			break;
		case SUCCES:
			ss << COL_SUCCES << "[SUCCES] "<< message << RESET << endl;
			break;
		case GET:
			ss << COL_GET << "[GET] "<< message << RESET << endl;
			break;
		case POST:
			ss << COL_SUCCES << "[POST] "<< message << RESET << endl;
			break;
		case DELETE:
			ss << COL_DELETE << "[DELETE] "<< message << RESET << endl;
			break;
		case ERROR:
			ss << COL_ERROR << "[ERROR] "<< message << RESET << endl;
			cerr << ss.str();
			return;
	}
	cout << ss.str();
}

void signal_handler(int signum) {
    (void)signum; 
    g_server_running = 0; 
	Console::message("SIGINT signal detected:", NOTICE, true);
	Console::message("Clean shutdown initialized.", NOTICE, false);
	cout << endl;
}

EpollManager::EpollManager(void) {  }

EpollManager::~EpollManager(void)
{
	map<int, Server*>::iterator it;

	for (it = this->servers_running.begin(); it != this->servers_running.end(); ++it)
		delete it->second;
	this->servers_running.clear();
	Console::message("Closing epoll file descriptor.", NOTICE, true);
	close(this->epoll_fd);
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

		stringstream ss;
		ss << "Port " << server->get_port() << " opened.";
		Console::message(ss.str(), INFO, true);

		this->servers_running.insert(make_pair(socket_fd, server));
	}
}

Client *accept_connection(Server *serv)
{
	int clientSocket = accept(serv->get_socket(), NULL, NULL);
	if (clientSocket == -1)
		return NULL;
	
	Client *client = new Client(clientSocket, serv);
	
	EpollManager &epoll_manager = serv->get_Epoll_Manager();
	if (clientSocket != -1)
	{
		stringstream ss;
		ss << "New Client connected on port " << serv->get_port() << ".";
		Console::message(ss.str(), INFO, false);

		int status = fcntl(clientSocket, F_SETFL, O_NONBLOCK);
		if (status == -1)
		{
			close(clientSocket);
			throw runtime_error("fcntl error");
		}
		struct epoll_event event;
		event.events = EPOLLIN | EPOLLOUT;
		event.data.fd = clientSocket;
		epoll_ctl(epoll_manager.get_Epoll_Fd(), EPOLL_CTL_ADD, clientSocket, &event);
	}
	return client;
}

string process_request(const string &request_str, Client &client)
{
	Parser parser(request_str);
	parser.parse_Request();
	
	Message_type type;	
	string method = parser.get_Method();
	if (method == "GET")
		type = GET;
	else if (method == "POST")
		type = POST;
	else if (method == "DELETE")
		type = DELETE;

	stringstream ss;
	ss << "Request recived: " << parser.get_Method() << " " << parser.get_Path() << " on port " << client.get_Server()->get_port() << ".";
	Console::message(ss.str(), type, true);
	if (parser.is_Valid())
		return "HTTP/1.1 200 OK\r\n\r\nHello World";
	else
		return "HTTP/1.1 400 OK\r\n\r\n";
}

void handle_Read(struct epoll_event &event, Client &client)
{
	char buffer[1024];
	
	while (true)
	{
		ssize_t count = read(event.data.fd, buffer, sizeof(buffer));		
		if (count == -1)
		{
			client.set_Response(process_request(client.get_Buffer(), client));
			stringstream portStr;

			portStr << endl << "Port: " << client.get_Server()->get_port();
			client.set_Response(client.get_Response() + portStr.str());
			break;
		}
		else if (count == 0)
		{
			stringstream ss;
			ss << "Client: " << event.data.fd << " disconnected." << endl;
			Console::message(ss.str(), INFO, false);

			close(event.data.fd);
			break;
		}
		else
			client.append_Buffer(buffer, count);
	}
}

bool handle_Write(struct epoll_event &event, Client &client)
{
	string response = client.get_Response();

	if (write(event.data.fd, response.c_str(), response.length()) != (ssize_t)response.length())
		Console::message("Invalid write.", ERROR, false);
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
					if (client == NULL)
					{
						Console::message("Client accept() fail, continue.", ERROR, false);
						continue;
					}
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
						delete it->second;
						client_map.erase(it);
					}
				}
			}
		}
	}
	map<int, Client*>::iterator it;
	for (it = client_map.begin(); it != client_map.end(); ++it)
	{
		close(it->first);
		delete it->second;
	}
	client_map.clear();
}

