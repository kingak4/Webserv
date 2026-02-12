#include "../../include/core/Client.hpp"

Client::Client(int socket, const Server *parent_server) : client_socket(socket), server(parent_server), buffer()
{
	cout << "client constructor!" << endl;
}

Client::Client(const Client &other) : client_socket(other.get_Socket()), server(other.get_Server()), buffer(other.get_Buffer())
{
}

Client::Client(void) : client_socket(-1), server(NULL)
{

}

void Client::set_Buffer(string &new_buffer)
{
	this->buffer = new_buffer;
}
const string &Client::get_Buffer(void) const
{
	return (this->buffer);
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		this->client_socket = other.get_Socket();
		this->server = other.get_Server();
		this->buffer = other.get_Buffer();
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
