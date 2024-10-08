#pragma once 
#include "Libs.hpp"
class Cgi {
public:
	// Constructor
	Cgi(std::string _programName, std::string _queryString, int _clientFd);

	// Destructor
	~Cgi() {}
	// Métodos adicionales de la clase
	void handlerCgi();
	std::string &get_output();

private:
	std::string programName;
	std::string queryString;
	int clientFd;
	std::string output;
};