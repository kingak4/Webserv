#include "Client.hpp"
#include "Server.hpp"


Client::Client(int socket, const Server *parent_server) : client_socket(socket), server(parent_server)
{
	cout << "client constructor!" << endl;
}

Client::Client(const Client &other) : client_socket(other.get_Socket()), server(other.get_Server())
{
}

Client::Client(void) : client_socket(-1), server(NULL)
{

}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		this->client_socket = other.get_Socket();
		this->server = other.get_Server();
	}
	return *this;
}

Client::~Client(void)
{
	cout << "client destructor!" << endl;
}

int Client::get_Socket(void) const
{
	return (client_socket);
}

const Server *Client::get_Server(void) const
{
	return (server);
}
