#ifndef PARSEUTILS_HPP
# define PARSEUTILS_HPP

# include "WebServer.hpp"

class ServerConfig;

int ft_stoi(std::string str);
std::string trimLeadingSpaces(const std::string& str);
std::vector<std::string> parseMethods(const std::string& input);
void parseIpAddress(std::string address, ServerConfig &sc);
void parseCmbs(std::string valueLine, ServerConfig &sc);

#endif