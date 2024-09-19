#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <iostream>
#include <fstream>


class FileExtensionException : public std::exception {
public:
    const char* what() const throw() {
        return "Error: Config file Extension not found";
    }
};

class FileLocationException : public std::exception {
public:
    const char* what() const throw() {
        return "Error: File not found in the config folder";
    }
};

class ServerConfig
{
    public:
        ServerConfig();
        ServerConfig(const ServerConfig &copy);
        virtual ~ServerConfig();
        ServerConfig &operator=(const ServerConfig &src);

    private:

};

void validFileName(const std::string& fileName);

#endif