#pragma once
#include "Libs.hpp"
#include <vector>
#include <map>
#include "WebServer.hpp"
#define MAX_EVENTS 64
class WebServer;
class ServerUp {
public:
	ServerUp(std::string &ip, size_t &port);
	ServerUp(const std::vector<ServerConfig>  &raw);
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
	std::vector<ServerConfig> list;
	std::vector<int> vSockets;
	void GenStruct(std::map<int, sockaddr_in> *servers, std::vector<int> *sockets);
	std::vector<int> get_SocketsOfServer();
	bool setupServerSocket(int serverSocket, const sockaddr_in& serverAddress);
	void newConect(int fd, int fdepoll);
	int checkfd(int fd);




};