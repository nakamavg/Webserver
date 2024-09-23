#pragma once
#include "../Incl/Libs.hpp"
#define MAGIC_NUMBER 64

class ServerUp {
public:
	ServerUp(std::string &ip, size_t &port);
	ServerUp(const std::string& str, unsigned long num);
	ServerUp();
	~ServerUp();
	void start();
	void stop();
	
private:
	std::string ip;
	size_t port;
	void GenStruct(sockaddr_in *setstruct);

};