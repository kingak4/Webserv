#include "../../include/core/Client.hpp"

Client::Client(int socket, const Server *parent_server) : client_socket(socket), server(parent_server), is_header_readed(false) ,is_chunked(false) {}

Client::Client(const Client &other) : client_socket(other.get_Socket()), server(other.get_Server()) {}

Client::Client(void) : client_socket(-1), server(NULL) {}

Client::~Client(void) {}

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

int Client::get_Socket(void) const
{
	return (client_socket);
}

<<<<<<< Updated upstream
=======
bool Client::get_is_header_readed(void)
{
	return (this->is_header_readed);
}

int Client::get_content_len(void)
{
	return (this->content_len);
}

void Client::set_is_header_readed(bool state)
{
	this->is_header_readed = state;
}

bool Client::get_is_chunked(void)
{
	return (this->is_chunked);
}

void Client::set_is_chunked(bool value)
{
	this->is_chunked = value;
}

void Client::set_content_len(int len)
{
	this->content_len = len;
}

>>>>>>> Stashed changes
const Server *Client::get_Server(void) const
{
	return (server);
}
