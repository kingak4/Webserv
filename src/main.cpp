#include "../include/core/EpollManager.hpp"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << RED << "Server launch failed: provide path to config.conf file." << RESET << endl;
		return 1;
	}
	else if (argc != 2)
	{
		cout << RED << "Server launch failed: too many arguments." << RESET << endl;
		return 1;
	}
	signal(SIGINT, signal_handler);
	string config_path = argv[1];
	EpollManager *epoll_manager = new EpollManager();
	vector<ServerData> splitted_config;

	try
	{
		ConfigParser configParser;
		configParser.parse_config_file(config_path);
		splitted_config = configParser.get_config_servers();

		epoll_manager->init_Epoll(splitted_config);
		epoll_manager->epoll_Loop();	
	}
	catch (runtime_error &e)
	{
		cout << RED << "ERROR: " << e.what() << RESET << endl;
	}

	delete  epoll_manager;
	return 0;
}
