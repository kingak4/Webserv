# ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include "../config/Route.hpp"
#include "../http/Request.hpp"

using namespace std;

class Route;

class CgiHandler
{
    private:
        Route& route;
        Request& request;

        string script_path;

    public:
        CgiHandler(Route& route, Request& request);
        ~CgiHandler();

        bool does_file_exist();
        string build_cgi_response(string& output);
        string trim(const string& str);
        string run();
};

#endif
