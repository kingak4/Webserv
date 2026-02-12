#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
using namespace std;

class Server;

class Client
{
	private:
		int client_socket;
		const Server *server;
		//TODO !!!!!
		//char buffer[1024];
	public:
		Client(int socket, const Server *server);
		Client(void);
		Client(const Client &other);
		~Client(void);
		Client &operator=(const Client &other);
		void set_Buffer(string &new_buffer);
		const string &get_Buffer(void) const;
		int get_Socket(void) const;
		const Server *get_Server(void) const;
};
#endif
