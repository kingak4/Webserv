#include "../../include/core/EpollManager.hpp"
#include <iterator>

// sig_atomic_t: an integer type that can be accessed atomically
volatile sig_atomic_t g_server_running = 1;

void signal_handler(int signum) {
    (void)signum; 
    g_server_running = 0; 
	cout << endl << BLUE << "SIGINT signal detected: " << BOLD_BLUE << "clean " << "shutdown initialized." << RESET << endl;
}

EpollManager::EpollManager(void) {  }

EpollManager::~EpollManager(void)
{
	map<int, Server*>::iterator it;

	for (it = this->servers_running.begin(); it != this->servers_running.end(); ++it)
		delete it->second;
	this->servers_running.clear();
	cout << BLUE << "Closing epoll file descriptor." << RESET << endl;
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
		cout << BLUE << "Port " << server->get_port() << " opened." << RESET << endl << endl;
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
		cout << "New Client connected on port " << serv->get_port() << "." << endl;
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

Route* get_route_block(ServerData server, Request& request, Config server_block)
{
    vector<Location>& locations = server.locations;
    string path = request.get_Path();
    string route_path;
    size_t pos;

    if ((pos = path.find("?")) != string::npos)
        route_path = path.substr(0, pos);
    else
        route_path = path;

    for (size_t i = 0; i < locations.size(); ++i)
    {
        if (route_path == locations[i].route_name)
            return new Route(locations[i], request, server_block);
    }
    return NULL;
}

string find_requested_server(Request& request, ConfigParser& config_parser, Client &client)
{
    string response;

    map<string, string> headers = request.get_Headers();
    vector<ServerData> servers = config_parser.get_config_servers();
    
    Config* server_block = NULL;
    Route* route_block = NULL;

	int client_port = client.get_Server()->get_port();

    vector<ServerData> candidates;
    for (size_t i = 0; i < servers.size(); ++i)
    {
        if (servers[i].port == client_port)
            candidates.push_back(servers[i]);
    }

    for (size_t i = 0; i < candidates.size(); i++)
    {
        if (candidates[i].server_name == headers["host"])
        {
            server_block = new Config(candidates[i], request);
            route_block = get_route_block(candidates[i], request, *server_block);
            break ;
        }
    }

    for (size_t i = 0; i < servers.size(); i++)
    {
        if (servers[i].server_name == headers["host"])
        {
            server_block = new Config(servers[i], request);
            route_block = get_route_block(servers[i], request, *server_block);
            break ;
        }
    }

    if (!server_block && !candidates.empty())
    {
        server_block = new Config(candidates[0], request);
        route_block = get_route_block(candidates[0], request, *server_block);
    }

    if (server_block && route_block)
    {
		// cout << "server_block: " << server_block->get_server_name() << server_block->get_port() << server_block->get_host() << endl;
		// cout << "route_block: " << route_block->get_route_name() << route_block->get_url() << endl;
        response = route_block->form_response();
        if (response == "")
        {
            cout << "response is NULL" << endl;
        }
		cout << "RESPONSE" << endl;
		cout << endl;
        cout << response << endl;

        delete server_block;
        delete route_block;
    }
	else
	{
		response += "HTTP/1.1 400 OK\r\n\r\nFailed";
	}
    
    return response;
}

string process_request(const string &request_str, Client &client, ConfigParser &config_parser)
{
	// ConfigParser config_parser;

	Parser parser(request_str);
	parser.parse_Request();
	
	Request request;
    request.buildFromParser(parser);

	// GET ROUTE BLOCK AND LOCATION
    return find_requested_server(request, config_parser, client);

	// cout << "Request recived: " << parser.get_Method() << " " << parser.get_Path() << " on port " << client.get_Server()->get_port() << "." << endl;
	// if (parser.is_Valid())
	// 	return "HTTP/1.1 200 OK\r\n\r\nHello World";
	// else
	// 	return "HTTP/1.1 400 OK\r\n\r\n";
}

void handle_Read(struct epoll_event &event, Client &client, ConfigParser &config_parser)
{
	char buffer[1024];
	
	while (true)
	{
		ssize_t count = read(event.data.fd, buffer, sizeof(buffer));		
		if (count == -1)
		{
			client.set_Response(process_request(client.get_Buffer(), client, config_parser));
			stringstream portStr;

			portStr << endl << "Port: " << client.get_Server()->get_port();
			client.set_Response(client.get_Response() + portStr.str());
			break;
		}
		else if (count == 0)
		{
			cout << "Client: " << event.data.fd << " disconnected." << endl;
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
		cout << "Invalid write" << endl;
	return true;
}

void EpollManager::epoll_Loop(ConfigParser &config_parser)
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
						cerr << RED << "Client accept() fail, continue.";
						continue;
					}
					client_map.insert(make_pair(client->get_Socket(), client));
				}
				else 
					handle_Read(active_events[i], *client_map[fd], config_parser);
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

