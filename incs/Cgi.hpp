#pragma once 
#include "Libs.hpp"
class Cgi {
public:
	// Constructor
	Cgi(std::string _programName, std::string _queryString);

	// Destructor
	~Cgi() {}
	// Métodos adicionales de la clase
	int handlerCgi();
	std:: string &cgiResponse (void);


private:
	std::string programName;
	std::string queryString;
	std::string output;
    std::string web;

};