#include "../incs/Cgi.hpp"
#include <unistd.h>
#include <signal.h>
void Cgi::getVariable(std::string variable)
{
   if (queryString.find(variable) != std::string::npos)
                {
                    userLogin = queryString;
                    userLogin.erase(0, userLogin.find(variable) + variable.size());
                    if (userLogin.find("&") != std::string::npos)
                        userLogin.erase(userLogin.find("&"), userLogin.size());
                }
}
int Cgi::handleParentProcess(int fdaux[2], pid_t pid)
{
    int status;
    char buffer[2048];

    close(fdaux[WRITE]);

    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        std::cerr << "The child proccess ended with an error.\n";
        return 400;
    }
    else
    {
        int nread = read(fdaux[READ], &buffer, sizeof(buffer));
        buffer[nread] = '\0';
        std::string aux(buffer);
        output = aux;

                std::cout << queryString << std::endl;
        getVariable("User=");
                std::cout << "The user is: " << userLogin << std::endl;
        return 0;
    }
}

void Cgi::executeChildProcess(int fdaux[2])
{
    char *argv[4];

    if (programName.find(".py") != std::string::npos)
        argv[0] = strdup(ROUTE_PYTHON);
    else if(programName.find(".php") != std::string::npos)
        argv[0] = strdup(ROUTE_PHP);
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

    exit(EXIT_FAILURE);
}

std::string &Cgi::cgiResponse(void)
{
    std::stringstream ss;
    ss << output.size();

    web =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n";
        std::cout <<"User login empy: = " << userLogin.empty() << std::endl;
    if (!userLogin.empty())
        web += "Set-Cookie: User= " + userLogin + "; Max-Age=3600; Path=/\r\n";
    web +=
        "Content-Length: " + ss.str() + "\r\n"
        "\r\n" + output.c_str();
    std::cout << "CGI response: " << web << std::endl;
    return web;
}

Cgi::Cgi(std::string _programName, std::string _queryString) : programName(_programName), queryString(_queryString)
{
}

int Cgi::handlerCgi()
{
    int fdaux[2];
    pipe(fdaux);
    pid_t pid = fork();

    if (pid == 0)
    {
        executeChildProcess(fdaux);
    }
    else if (pid > 0)
    {
        pid_t timeout_pid = fork();

        if (timeout_pid == 0)
        {
            sleep(TIMEOUT); 
            kill(pid, SIGKILL); 
            exit(EXIT_SUCCESS);
        }
        else if (timeout_pid > 0)
        {
     
            int status;
            waitpid(pid, &status, 0); 

            if (WIFEXITED(status))
            {
          
                kill(timeout_pid, SIGKILL);
                return handleParentProcess(fdaux, pid);
            }
            else
            {
                std::cerr << "The child CGI proccess ended in a timeout.\n";
                return 408; 
            }
        }
        else
        {
            std::cerr << "The fork of the timeout proccess failed\n";
            return 500;
        }
    }
    else
    {
        std::cerr << "Fork falló.\n";
        return 500;
    }
    return 0;
}