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
		string buffer;
		string resopnse;
		bool is_header_readed;
		int content_len;
	public:
		Client(int socket, const Server *server);
		Client(void);
		Client(const Client &other);
		~Client(void);
		Client &operator=(const Client &other);
		void append_Buffer(const char *data, size_t size);
		const string &get_Buffer(void) const;
		const string &get_Response(void) const;
		void set_Response(const string &new_response);
		int get_Socket(void) const;
		bool get_is_header_readed(void);
		int get_content_len(void);
		void set_is_header_readed(bool state);
		void set_content_len(int len);
		const Server *get_Server(void) const;
};
#endif
