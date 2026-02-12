// #include "../../include/cgi/CgiHandler.hpp"

// CgiHandler::CgiHandler(Route& route, Request &request)
// {
//     route = route;
//     request = request;
// }

// CgiHandler::~CgiHandler() {}

// bool CgiHandler::does_file_exist()
// {
//     struct stat st;

//     if (stat(route.filesystem_path.c_str(), &st) == 0 && S_ISREG(st.st_mode))
//         return true;
//     return false;
// }

// void CgiHandler::run()
// {
//     bool file_exist;
//     pid_t pid;
//     int in_pipe[2];
//     int out_pipe[2];
//     char *argv[3];

//     // 1. check if the file exist and is normal file
//     file_exist = does_file_exist();
//     if (!file_exist)
//         return ;
    
//     cout << "filesystem_path: " << route.filesystem_path << endl;
//     // 2. create pipes
//     if (pipe(in_pipe) == -1)
//         return ;
//     if (pipe(out_pipe) == -1)
//     {
//         close(in_pipe[0]);
//         close(in_pipe[1]);
//         return ;
//     } 

//     // 3. create a child process using fork
//     pid = fork();

//     if (pid == 0)
//     {
//         dup2(in_pipe[0], STDIN_FILENO);
//         dup2(out_pipe[1], STDOUT_FILENO);

//         close(in_pipe[1]);
//         close(out_pipe[0]);

//         char *argv[3];
//         argv[0] = strdup("/usr/bin/env python3");
//         argv[1] = strdup(filesystem_path.c_str());
//         argv[0] = NULL;
//         execve(argv[0], argv, envp);
//         exit(1);
//     }
//     else
//         waitpid(pid, NULL, 0);
// }

