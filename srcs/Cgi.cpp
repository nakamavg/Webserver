#include "../incs/Cgi.hpp"

Cgi::Cgi(std::string _programName, std::string _queryString) : programName(_programName), queryString(_queryString)
{}
std::string & Cgi::get_output(){return this->output ;}

int Cgi::handlerCgi()
{
    int fdaux[2];
    pipe(fdaux);

    pid_t pid = fork(); 

    if (pid == 0) 
    {
        char *argv[4];
                std::cout <<"el nombre de l programa es : " << programName <<std::endl;
        if(programName.find(".py") != std::string::npos)
            argv[0] = strdup("/usr/bin/python3");
        else
            argv[0] = strdup(programName.c_str());
        argv[1] = strdup(programName.c_str()); 
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
            return(400);
        }
        else{
        int nread=read(fdaux[READ],&buffer ,sizeof(buffer));
        buffer[nread] = '\0';
        std::string aux(buffer);
        output = aux;
        return (200);
        }
    }
    else
    {
        std::cerr << "Fork falló.\n";
    }
    return 0;
}
