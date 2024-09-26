#pragma once
#include "../Incl/Libs.hpp"
#include "serverlist.hpp"
#include <vector>
#define MAGIC_NUMBER 64
class serverlist;
class ServerUp {
public:
	ServerUp(std::string &ip, size_t &port);
	ServerUp(std::vector<serverlist>  &raw);
	ServerUp(const std::string& str, unsigned long num);
	ServerUp();
	~ServerUp();
	void start();
	void stop();
	size_t getNservers();
private:
	std::string ip;
	size_t port;
	size_t nServers;
	void GenStruct(sockaddr_in *setstruct);
	std::vector<int> get_SocketsOfServer();


};