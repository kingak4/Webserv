#include "../../include/cgi/CgiHandler.hpp"

CgiHandler::CgiHandler(Route& route, Request& request) : route(route), request(request) {}

CgiHandler::~CgiHandler() {}

bool CgiHandler::does_file_exist()
{
    struct stat st;

    if (stat(route.get_filesystem_path().c_str(), &st) == 0 && S_ISREG(st.st_mode))
        return true;
    return false;
}

string CgiHandler::run()
{
    bool file_exist;
    pid_t pid;
    int in_pipe[2];
    int out_pipe[2];
    char *argv[3];
    vector<string> envp_str;
    vector<char*> envp;
    string body;

    // 1. check if the file exist and is normal file
    file_exist = does_file_exist();
    if (!file_exist)
        return NULL;
    
    cout << "filesystem_path: " << route.get_filesystem_path() << endl;
    // 2. create pipes
    if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1)
        return NULL;

    // 3. create a child process using fork
    pid = fork();
    if (pid < 0)
        return NULL;

    if (pid == 0)
    {
        dup2(in_pipe[0], STDIN_FILENO);
        dup2(out_pipe[1], STDOUT_FILENO);

        close(in_pipe[1]);
        close(out_pipe[0]);

        // BUILD ARGV
        argv[0] = strdup("/usr/bin/env python3");
        argv[1] = strdup(route.get_filesystem_path().c_str());
        argv[2] = NULL;

        // BUILD CGI environment
        envp_str.push_back("REQUEST_METHOD=" + request.get_Method());
        envp_str.push_back("QUERY_STRING=" + route.get_request_query());
        envp_str.push_back("CONTENT_LENGTH=" + request.get_Headers()["Content-Length"]);
        envp_str.push_back("CONTENT_TYPE=" + request.get_Headers()["Content-Type"]);
        envp_str.push_back("SCRIPT_NAME=" + request.get_Path());
        envp_str.push_back("QUERY_STRING=" + route.get_request_query());
        envp_str.push_back("SERVER_PROTOCOL=" + request.get_Version());
        envp_str.push_back("GATEWAY_INTERFACE=CGI/1.1");

        for (size_t i = 0; i < envp_str.size(); ++i)
            envp.push_back(strdup(envp_str[i].c_str()));
        envp.push_back(NULL);

        execve(argv[0], argv, &envp[0]);
        exit(1);
    }
    else
    {
        close(in_pipe[0]); // parent doesn't read from input pipe
        close(out_pipe[1]); // parent doesn't write to output pipe

        if (request.get_Method() == "POST")
        {
            body = request.get_Body();
            write(in_pipe[1], body.c_str(), body.size());
        }
        close(in_pipe[1]); // send EOF to CGI
        waitpid(pid, NULL, 0);

        // read CGI output
        char buffer[4096];
        size_t bytes;
        string output;

        while ((bytes = read(out_pipe[0], buffer, sizeof(buffer))) > 0)
            output.append(buffer, bytes);
        
        close(out_pipe[0]);

        return output;
    }
}
