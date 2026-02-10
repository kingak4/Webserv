#ifndef CLIENT_HPP
#define CLIENT_HPP

class Server;

class Client
{
	private:
		int client_socket;
		const Server *server;
	public:
		Client(int socket, const Server *server);
		Client(void);
		Client(const Client &other);
		~Client(void);
		Client &operator=(const Client &other);
		int get_Socket(void) const;
		const Server *get_Server(void) const;
};
#endif
