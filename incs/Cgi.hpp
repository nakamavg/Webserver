#pragma once 
#include "Libs.hpp"
#define TIMEOUT 5
#define ROUTE_PHP "/usr/bin/php"
#define ROUTE_PYTHON "/usr/bin/python3"

class Cgi {
public:
	// Constructor
	Cgi(std::string _programName, std::string _queryString);

	// Destructor
	~Cgi() {}
	// Métodos adicionales de la clase
	int handlerCgi();
	std::string cgiResponse(std::string c);
	std:: string &cgiResponse (void);
	void executeChildProcess(int fdaux[2]);
	int handleParentProcess(int fdaux[2], pid_t pid);
	void getVariable(std::string variable);

private:
	std::string programName;
	std::string queryString;
	std::string output;
    std::string web;
	std::string userLogin;
	std::string cookie;

};