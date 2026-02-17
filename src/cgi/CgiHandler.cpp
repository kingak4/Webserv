#include "../../include/cgi/CgiHandler.hpp"

CgiHandler::CgiHandler(Route& route, Request& request) : route(route), request(request) {}

CgiHandler::~CgiHandler() {}

bool CgiHandler::does_file_exist()
{
    struct stat st;
    // string script;
    // string path_to_script;

    // script = route.get_request_path();
    // cout << "script: " << script << endl;
    // path_to_script = route.get_filesystem_path() + script;
    // cout << "path_to_script: " << path_to_script << endl;
    if (stat(route.get_filesystem_path().c_str(), &st) == 0 && S_ISREG(st.st_mode))
        return true;
    return false;
}

string CgiHandler::trim(const string& str)
{
    size_t start;
    size_t end;
    
    if (str.empty())
        return "";

    start = 0;
    end = str.size() - 1;

    while (start <= end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n'))
        ++start;

    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n'))
        --end;

    return str.substr(start, end - start + 1);
}

string CgiHandler::build_cgi_response(string& output)
{
    string headers;
    string body;
    size_t pos;
    string content_type;
    string line;

    body = output;
    pos = output.find("\n\n");
    if (pos != string::npos)
    {
        headers = output.substr(0, pos);
        body = output.substr(pos + 2);
    }

    content_type = "text/html";
    istringstream hs(headers);
    
    while (getline(hs, line))
    {
        if (line.find("Content-Type:") != string::npos)
        {
            content_type = line.substr(13);
            content_type = trim(content_type);
            break ;
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
    char *argv[3];
    vector<string> envp_str;
    vector<char*> envp;
    string body;

    // 1. check if the file exist and is normal file
    file_exist = does_file_exist();
    if (!file_exist)
        return "FILE DOESN'T EXIST";

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
        argv[0] = strdup("/usr/bin/env");
        argv[1] = strdup("python3");
        argv[2] = strdup(route.get_filesystem_path().c_str());
        argv[3] = NULL;

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
        cout << "output: " << output << endl;  
        return output;
    }
}
