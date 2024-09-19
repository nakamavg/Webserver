#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

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

class configFile
{
    public:
        configFile();
        configFile(const configFile &copy);
        virtual ~configFile();
        configFile &operator=(const configFile &src);

    private:

};

void validFileName(const std::string& fileName);

#endif