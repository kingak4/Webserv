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

Route* get_route_block(ServerData server, Request& request, Config& server_block)
{
    vector<Location>& locations = server.locations;
    string path = request.get_Path();
    string route_path;
    size_t pos;
	Location* best_match = NULL;
	size_t longest = 0;

    if ((pos = path.find("?")) != string::npos)
        route_path = path.substr(0, pos);
    else
        route_path = path;

    for (size_t i = 0; i < locations.size(); ++i)
    {
        string& loc = locations[i].route_name;

        if (route_path.compare(0, loc.length(), loc) == 0)
        {
            if (loc.length() > longest)
            {
                longest = loc.length();
                best_match = &locations[i];
            }
        }
	}
    if (best_match)
        return new Route(*best_match, request, server_block);

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
		// cout << "server_name: " << server_block->get_server_name() << endl;
		// cout << "server_block: " << server_block->get_root_dir() << endl;
		// cout << "route_block: " << route_block->get_route_name() << route_block->get_url() << endl;
        response = route_block->form_response();
        if (response == "")
        {
            cout << "response is NULL" << endl;
        }
		cout << endl;
        //cout << response << endl;

        delete server_block;
        delete route_block;
    }
	else
	{
		if (server_block)
			delete server_block;
		if (route_block)
			delete route_block;
		response += "HTTP/1.1 400 OK\r\n\r\nFailed";
	}
    
    return response;
}

string process_request(const string &request_str, Client &client, ConfigParser &config_parser)
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
	// if (parser.is_Valid())
	// 	return "HTTP/1.1 200 OK\r\n\r\nHello World";
	// else
	// 	return "HTTP/1.1 400 OK\r\n\r\n";
	Request request;
    request.buildFromParser(parser);

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
	ssize_t printed;

	if (response.empty())
		return true;
	printed = write(event.data.fd, response.c_str(), response.length());
	if (printed <= 0)
		return true;
	if (printed < (ssize_t)response.length())
	{
		response.erase(0, printed);
		client.set_Response(response);
		return false;
	}
	else
	{
		client.set_Response("");
		return true;
	}
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
						Console::message("Client accept() fail, continue.", ERROR, false);
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

