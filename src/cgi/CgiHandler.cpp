#include "../../include/cgi/CgiHandler.hpp"
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

CgiHandler::CgiHandler(Route& route, Request& request) : route(route), request(request) {}

CgiHandler::~CgiHandler() {}

CgiHandler::CgiException::CgiException(const string& msg) : runtime_error(msg) {}

CgiHandler::CgiException::~CgiException() throw() {}

bool CgiHandler::does_file_exist()
{
    struct stat st;
    if (stat(route.get_filesystem_path().c_str(), &st) == 0 && S_ISREG(st.st_mode))
        return true;
    return false;
}

string CgiHandler::trim(const string& str)
{
    size_t start = 0;
    size_t end = str.size();
    if (str.empty())
        return "";

    end--;

    while (start <= end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n'))
        ++start;

    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n'))
        --end;

    return str.substr(start, end - start + 1);
}

string CgiHandler::build_cgi_response(std::string& output)
{
    string headers;
    string body = output;
    size_t pos = output.find("\n\n");
    if (pos != string::npos)
    {
        headers = output.substr(0, pos);
        body = output.substr(pos + 2);
    }

    string content_type = "text/html";
    istringstream hs(headers);
    string line;
    while (getline(hs, line))
    {
        if (line.find("Content-Type:") != string::npos)
        {
            content_type = trim(line.substr(13));
            break;
        }
    }

    ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body;

    return response.str();
}

string CgiHandler::run()
{
    bool file_exist;
    pid_t pid;
    int in_pipe[2];
    int out_pipe[2];
    char *argv[4];
    vector<std::string> envp_str;
    vector<char*> envp;
    string body;

    file_exist = does_file_exist();
    if (!file_exist)
        return route.error_response("404");

    if (request.get_Method() == "POST")
        body = request.get_Body();

    if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1)
        throw CgiException("Error: pipe creation failed.");

    pid = fork();
    if (pid < 0)
        throw CgiException("Error: fork failed");

    if (pid == 0)
    {
        dup2(in_pipe[0], STDIN_FILENO);
        dup2(out_pipe[1], STDOUT_FILENO);

        close(in_pipe[1]);
        close(out_pipe[0]);

        argv[0] = strdup("/usr/bin/env");
        argv[1] = strdup("python3");
        argv[2] = strdup(route.get_filesystem_path().c_str());
        argv[3] = NULL;

        envp_str.push_back("REQUEST_METHOD=" + request.get_Method());
        envp_str.push_back("SCRIPT_NAME=" + request.get_Path());
        envp_str.push_back("SERVER_PROTOCOL=" + request.get_Version());
        envp_str.push_back("GATEWAY_INTERFACE=CGI/1.1");

        envp_str.push_back("QUERY_STRING=" + route.get_request_query());

        if (request.get_Method() == "POST")
        {
            ostringstream oss;
            oss << body.size();
            envp_str.push_back("CONTENT_LENGTH=" + oss.str());

            string content_type = "application/x-www-form-urlencoded";
            if (request.get_Headers().count("Content-Type"))
                content_type = request.get_Headers()["Content-Type"];
            envp_str.push_back("CONTENT_TYPE=" + content_type);
        }

        for (size_t i = 0; i < envp_str.size(); ++i)
            envp.push_back(strdup(envp_str[i].c_str()));
        envp.push_back(NULL);

        execve(argv[0], argv, &envp[0]);
        perror("execve failed");
        exit(1);
    }
    else
    {
        close(in_pipe[0]);
        close(out_pipe[1]);

        if (request.get_Method() == "POST" && !body.empty())
        {
            size_t total = 0;
            ssize_t n;
            while (total < body.size())
            {
                n = write(in_pipe[1], body.c_str() + total, body.size() - total);
                if (n <= 0)
                {
                    perror("Write to CGI stdin failed");
                    break;
                }
                total += n;
            }
        }
        close(in_pipe[1]);

        waitpid(pid, NULL, 0);

        char buffer[4096];
        size_t bytes;
        string output;
        while ((bytes = read(out_pipe[0], buffer, sizeof(buffer))) > 0)
            output.append(buffer, bytes);

        close(out_pipe[0]);
        cout << "output: " << output << std::endl;
        return output;
    }
}