// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	int status = fcntl(clientSocket, F_SETFL, fcntl(clientSocket, F_GETFL, 0) | O_NONBLOCK);
	if (status == -1)
		std::cerr << "fcntl error\n";

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));

	std::stringstream ss;
	ss << "Hello, server! Client: " << argv[1];
	std::string temp_str = ss.str();
    // sending data
    const char* message = temp_str.c_str();
    send(clientSocket, message, strlen(message), 0);

    // closing socket
    close(clientSocket);

    return 0;
}
