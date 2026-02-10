#include "../include/core/EpollManager.hpp"

int main()
{
	string aaa = "abc";
	EpollManager *epoll_manager = new EpollManager(aaa);
	try
	{
		epoll_manager->epoll_Loop();	
	}
	catch (runtime_error &e)
	{
		cout << RED << "ERROR: " << e.what() << RESET << endl;
	}
	delete  epoll_manager;
	return 0;
}
