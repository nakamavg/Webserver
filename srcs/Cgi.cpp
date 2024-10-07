#include "../incs/Cgi.hpp"

Cgi::Cgi(std::string _programName, std::string _queryString, int _clientFd) : programName(_programName), queryString(_queryString),clientFd(_clientFd)
{}

std::string &Cgi::handlerCgi()
{
    int fdaux[2];
    pipe(fdaux);

    pid_t pid = fork(); 

    if (pid == 0) 
    {
        char *argv[4];
        argv[0] = strdup("/usr/bin/python3");
        argv[1] = strdup("/sgoinfre/students/dgomez-m/webserverdeverdadelabuena/cgi/hy.py"); 
        argv[2] = NULL;

        std::string query_string_env = "QUERY_STRING=" + queryString;
        char *envp[] = {
            strdup(query_string_env.c_str()),
            NULL
        };
        close(fdaux[READ]); 
        dup2(fdaux[WRITE], STDOUT_FILENO);
        close(fdaux[WRITE]); 
        execve(argv[0], argv, envp);

       // perror(""); 
        exit(EXIT_FAILURE); 
    }
    else if (pid > 0)
    {
        int status;
        char buffer[2048];
        close(fdaux[WRITE]);
        waitpid(pid, &status, 0)
        ;
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            std::cerr << "El proceso hijo terminó con un error.\n";
        }
        int nread=read(fdaux[READ],&buffer ,sizeof(buffer));
        buffer[nread] = '\0';
        std::string manolo(buffer);

        
        return(manolo);
    }
    else
    {
        std::string puta = "puta";
        std::cerr << "Fork falló.\n";
        return (puta);
    }
}