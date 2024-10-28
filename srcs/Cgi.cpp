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

    // Cerrar el extremo de escritura del pipe
    close(fdaux[WRITE]);

    // Esperar a que el proceso hijo termine
    waitpid(pid, &status, 0);

    // Comprobar si el proceso hijo terminó con éxito
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        std::cerr << "El proceso hijo terminó con un error.\n";
        return 400;
    }
    else
    {
        // Leer la salida del proceso hijo
        int nread = read(fdaux[READ], &buffer, sizeof(buffer));
        buffer[nread] = '\0';
        std::string aux(buffer);
        output = aux;

                std::cout << queryString << std::endl;
        getVariable("User=");
                std::cout << "El usuario es: " << userLogin << std::endl;
        return 0;
    }
}

void Cgi::executeChildProcess(int fdaux[2])
{
    char *argv[4];
    std::cout << "El nombre del programa es: " << programName << std::endl;

    // Configuración del comando a ejecutar según el tipo de archivo
    if (programName.find(".py") != std::string::npos)
        argv[0] = strdup(ROUTE_PYTHON);
    else if(programName.find(".php") != std::string::npos)
        argv[0] = strdup(ROUTE_PHP);
    else
        argv[0] = strdup(programName.c_str());

    argv[1] = strdup(programName.c_str());
    argv[2] = NULL;

    // Configuración de las variables de entorno
    std::string query_string_env = "QUERY_STRING=" + queryString;
    std::cout<<queryString << std::endl;
    char *envp[] = {
        strdup(query_string_env.c_str()),
        NULL
    };

    // Redirección de la salida estándar al pipe
    close(fdaux[READ]);
    dup2(fdaux[WRITE], STDOUT_FILENO);
    close(fdaux[WRITE]);

    // Ejecución del programa CGI
    execve(argv[0], argv, envp);

    // En caso de error, se cierra el proceso con un código de fallo
    exit(EXIT_FAILURE);
}

std::string &Cgi::cgiResponse(void)
{
    std::stringstream ss;
    ss << output.size();

    web =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n";
        std::cout <<"el userlogin.empty devuelve = " << userLogin.empty() << std::endl;
    if (!userLogin.empty())
        web += "Set-Cookie: User= " + userLogin + "; Max-Age=3600; Path=/\r\n";
    web +=
        "Content-Length: " + ss.str() + "\r\n"
        "\r\n" + output.c_str();
    std::cout << "La respuesta del CGI es: " << web << std::endl;
    return web;
}

Cgi::Cgi(std::string _programName, std::string _queryString) : programName(_programName), queryString(_queryString)
{
    std::cout << "El programa es: " << programName << std::endl;
    std::cout << "La query es: " << queryString << std::endl;
}

int Cgi::handlerCgi()
{
    int fdaux[2];
    pipe(fdaux);
    std::cout << "la query de mi amor es: " << queryString << std::endl;
    pid_t pid = fork();

    if (pid == 0)
    {
        // Proceso hijo: ejecutar el proceso CGI
        executeChildProcess(fdaux);
    }
    else if (pid > 0)
    {
        // Proceso padre: manejar el timeout
        pid_t timeout_pid = fork();

        if (timeout_pid == 0)
        {
            // Proceso hijo del timeout: dormir por el tiempo de timeout
            sleep(5); // Timeout de 5 segundos
            kill(pid, SIGKILL); // Matar el proceso CGI si aún está corriendo
            exit(EXIT_SUCCESS);
        }
        else if (timeout_pid > 0)
        {
            // Proceso padre: manejar el proceso CGI y el proceso de timeout
            int status;
            waitpid(pid, &status, 0); // Esperar a que el proceso CGI termine

            if (WIFEXITED(status))
            {
                // Si el proceso CGI terminó, matar el proceso de timeout
                kill(timeout_pid, SIGKILL);
                return handleParentProcess(fdaux, pid);
            }
            else
            {
                std::cerr << "El proceso CGI fue terminado por el timeout.\n";
                return 408; // Request Timeout
            }
        }
        else
        {
            std::cerr << "Fork falló para el proceso de timeout.\n";
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