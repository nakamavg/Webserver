#pragma once
#include "Libs.hpp"
#include <vector>
#include <map>
#include "WebServer.hpp"
#include "ParseRequest.hpp"
#include "Response.hpp"
#include "Cgi.hpp"



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
	std::map<int, size_t> clientVport;
	void GenStruct(std::map<int, sockaddr_in> *servers, std::vector<int> *sockets, std::map<int,ServerConfig> *serverPort);
	std::vector<int> get_SocketsOfServer();
	bool setupServerSocket(int serverSocket, const sockaddr_in& serverAddress);
	void newConect(int serverfd, int fdEpoll, std::map<int, ServerConfig> &serverPort, std::map<int, ServerConfig> &clientPort);
	int checkfd(int fd);


	std::string	readHttpRequest( int socket );


};