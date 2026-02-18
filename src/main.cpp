#include "../include/core/EpollManager.hpp"

int main(int argc, char **argv)
{
    ConfigParser config_parser;

	signal(SIGINT, signal_handler);
	// string config_path = argv[1];
	EpollManager *epoll_manager = new EpollManager();
	vector<ServerData> splitted_config;

	try
	{
		cout << BOLD_BLUE << "Server initialization" << RESET << endl;
		//TODO Config parser needs to throw an exceptions on errors
		ConfigParser configParser;

		string default_config_path = "config/default.conf";
		if (argc == 1)
			configParser.parse_config_file(default_config_path);
		else if (argc == 2)
		{
			string config_path = argv[1];
			configParser.parse_config_file(config_path);
		}
		else
		{
			cout << "Wrong number of argument" << endl;
        	return 1;
		}
	
		splitted_config = configParser.get_config_servers();
		cout << BLUE << "Config correct." << RESET << endl;

		epoll_manager->init_Epoll(splitted_config);
		cout << BLUE << "Epoll initialization succesful." << RESET << endl;
		cout << GREEN << "Server initialized succesfully." << RESET << endl;
		epoll_manager->epoll_Loop(configParser);	
	}
	catch (runtime_error &e)
	{
		cerr << RED << "ERROR: " << e.what() << RESET << endl;
		cout << RED << "Server shutdown. Disabling " << epoll_manager->get_Servers_Running().size() << " ports." << RESET << endl; 
	}

	try
	{
		delete epoll_manager;
		cout << GREEN << "Server disabled cleanly. No issues detected." << RESET << endl;
	}
	catch (runtime_error &e)
	{
	}
	return 0;
}
