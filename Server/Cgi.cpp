#include "Cgi.hpp"
void Cgi::executeCGIScript(const char* scriptPath, char* const argv[], char* const envp[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        if (execve(scriptPath, argv, envp) == -1) {
            perror("execve failed");
            _exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0); 
    }
}