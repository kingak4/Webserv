#include "../include/core/EpollManager.hpp"

int main(int argc, char **argv)
{
	signal(SIGINT, signal_handler);
	// string config_path = argv[1];
	EpollManager *epoll_manager = new EpollManager();
	vector<ServerData> splitted_config;

	try
	{
		Console::message("Server initialization", NOTICE, false);
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
		Console::message("Config correct", SUCCES, false);
		cout << endl;

		epoll_manager->init_Epoll(splitted_config);
		Console::message("Epoll initialization succesful.", INFO, false);
		Console::message("Server initialized succesfully.", SUCCES, true);
		epoll_manager->epoll_Loop(configParser);	
	}
	catch (runtime_error &e)
	{
		stringstream ss;
		ss << "ERROR: " << e.what();
		Console::message(ss.str(), ERROR, true);
		ss.clear();
		
		ss << "Server shutdown. Disabling " << epoll_manager->get_Servers_Running().size() << " ports.";
		Console::message(ss.str(), ERROR, false);
	}

	try
	{
		delete epoll_manager;
		Console::message("Server disabled cleanly. No issues detected.", SUCCES, true);
	}
	catch (runtime_error &e)
	{
	}
	return 0;
}
