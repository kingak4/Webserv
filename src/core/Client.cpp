#include "../../include/core/Client.hpp"

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

const string &Client::get_Response(void) const
{
	return this->resopnse;
}

void Client::set_Response(const string &new_response)
{
	this->resopnse.assign(new_response);	
}

void Client::append_Buffer(const char *data, size_t size)
{
	this->buffer.append(data, size);
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
		this->buffer = other.get_Buffer().c_str();
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
