#include "../incs/ServerUp.hpp"
int g_sig = 0;

void	ServerUp::setSig(int i)
{
	g_sig = i;
}

void ServerUp::sigHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";

	g_sig = 1;
    return ;
}

int ServerUp::checkfd(int fd)
{
for (std::vector<int>::iterator it = vSockets.begin(); it != vSockets.end(); ++it)
	{
		if(fd == *it)
			return fd;
	}
	return 0;
}
int		setsocknonblock(int sock)
{
	int flag;

	flag = fcntl(sock, F_GETFL, 0);
	if (flag < 0)
	{
		std::cerr << "fnclt" << std::endl;
		return (0);
	}
	if (fcntl(sock, F_SETFL, flag | O_NONBLOCK) < 0)
	{
		std::cerr << "fnctl" << std::endl;
		return (0);
	}
	return (1);
}
bool ServerUp::setupServerSocket(int serverSocket,
	const sockaddr_in &serverAddress)
{
	int	option;

	option = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
			&option, sizeof(option)) < 0)
	{
		std::cerr << "Set Sock Error" << std::endl;
		close(serverSocket);
		return (false);
	}
	if (bind(serverSocket, (struct sockaddr *)&serverAddress,
			sizeof(serverAddress)) < 0)
	{
		std::cerr << "Bind Error" << std::endl;
		close(serverSocket);
		return (false);
	}
	if (listen(serverSocket, SOMAXCONN) < 0)
	{
		std::cerr << "Listen Error" << std::endl;
		return (false);
	}
	return (true);
}

void ServerUp::GenStruct(std::map<int, sockaddr_in> *servers,
	std::vector<int> *sockets,std::map<int,ServerConfig> *serverPort)
{
	size_t			i;
	sockaddr_in	serverAddress;

	i = 0;
	while (i < this->nServers)
	{
		::bzero(&serverAddress, sizeof(sockaddr_in));
		serverAddress.sin_family = AF_INET;

		struct addrinfo hints, *res;
		::bzero(&hints, sizeof(hints));
		hints.ai_family = AF_INET; // IPv4
		hints.ai_socktype = SOCK_STREAM;

		int status = getaddrinfo(list[i].getHost().c_str(), NULL, &hints, &res);
		if (status != 0)
		{
			std::cout <<" hola "<<list[i].getHost() << std::endl;
			std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
			// Manejar el error adecuadamente
			list.erase(list.begin() + i);
			nServers--;
			continue;
		}

		struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
		serverAddress.sin_addr = ipv4->sin_addr;
		serverAddress.sin_port = htons(list[i].getPort());

		servers->insert(std::make_pair((*sockets)[i], serverAddress));
		serverPort->insert(std::make_pair((*sockets)[i], list[i]));

		freeaddrinfo(res); // Liberar la memoria asignada por getaddrinfo
		i++;
	}
}
void ServerUp::newConect(int serverfd, int fdEpoll,std::map<int,ServerConfig> &serverPort,std::map<int, ServerConfig> &clientPort)
{
	epoll_event ev;
	sockaddr in_addr;
	socklen_t in_addr_len = sizeof(in_addr);
	int newfd;
	if ((newfd = accept(serverfd, (struct sockaddr *)&in_addr, &in_addr_len)) < 0)
	{
		if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
		{
				std::cout << "we already processed all incoming connections" << std::endl;
		}
		else
		{
			std::cerr << "accept()" << std::endl;
		}
	}
		setsocknonblock(newfd);
	::bzero(&ev,sizeof(ev));
		clientPort[newfd] = serverPort[serverfd];
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd= newfd;
	if(epoll_ctl(fdEpoll,EPOLL_CTL_ADD,newfd, &ev)< 0)
		std::cerr << "epoll control" << std::endl;

}

ServerUp::ServerUp(const std::string &ip, size_t port) : ip(ip), port(port)
{
	_sig = 0;
}

size_t ServerUp::getNservers()
{
	return (this->nServers);
}

ServerUp::ServerUp(const std::vector<ServerConfig> &raw) : nServers(0), list(raw)
{
	size_t	nserv;


	nserv = 0;
	std::vector<ServerConfig>::iterator pailan = list.begin();
	while (pailan != list.end())
	{
		std::cout << "You can " << GREEN << "connect" << NC <<" to:" << std::endl;
		std::cout << (*pailan).getHost() <<  ":" << (*pailan).getPort() << std::endl;

		*pailan++;
		nserv++;
	}
	this->nServers = nserv;
	_sig = 0;
}

std::vector<int> ServerUp::get_SocketsOfServer()
{
	size_t	i;
	int	serverSocket;

	std::vector<int> sockets;
	i = 1;
	while (i <= this->nServers)
	{
		if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::cerr << errno << std::endl;
			continue ;
		} 
		sockets.push_back(serverSocket);
		i++;
	}
	return (sockets);
}

void ServerUp::start()
{
	signal(SIGINT, sigHandler);
	epoll_event	evClient[MAX_EVENTS];
	int			fdac;
	std::string response;
	epoll_event	ev;


	std::map<int, sockaddr_in> se;
	std::map<int, ServerConfig> serverPort;
	std::map<int, ServerConfig> clientPort;

	if (_sig == 1)
		return ;
	vSockets = get_SocketsOfServer();
	GenStruct(&se, &vSockets, &serverPort);
	epoll_fd = epoll_create(MAX_EVENTS);
	
	if (epoll_fd == -1)
	{
		std::cerr << "" << std::endl;
		return ;
	}
	for (std::vector<int>::iterator it = vSockets.begin(); it != vSockets.end(); ++it)
	{
		if (!setupServerSocket(*it, se[*it]))
		{
			vSockets.erase(it);
			it--;
			continue ;
		}
		::bzero(&ev,sizeof(ev));	
		ev.events = EPOLLIN | EPOLLOUT; 
		ev.data.fd = *it;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, *it, &ev) == -1)
		{
			std::cerr << "" << std::endl;
			close(*it);
			continue ;
		}
	}
	if (vSockets.size() == 0)
		return ;
	std::cout << "Number of servers: " << nServers << std::endl;
	std::cout << "Waiting for connections..." << std::endl;
	while (42)
	{
			if(nServers == 0)
				break;
			if(g_sig == 1)
				break;
			fdac = epoll_wait(epoll_fd, evClient, MAX_EVENTS, -1);
			if (fdac == -1)
			{
				std::cerr << "epoll_wait failed" << std::endl;
				return ;
			}
			for(int n = 0;n < fdac; n++)
			{
				if(int fdconnect = checkfd(evClient[n].data.fd))
				{	
					newConect(fdconnect,epoll_fd, serverPort, clientPort);
					break;
				}
				if(evClient[n].events & EPOLLIN)
				{
						
					std::string		request;
					Response		response(clientPort[evClient[n].data.fd]);

					request = readHttpRequest(evClient[n].data.fd);

					if (evClient[n].events & EPOLLOUT)
					{
						if (_reqErr < 0)
						{
							std::cout << "Error: Recv failed" << std::endl;
							handle_request_error(500, evClient[n], response);
						}
						else if (_reqErr == 0)
						{
							std::cout << "Connection is closed" << std::endl;
							handle_request_error(0, evClient[n], response);
						}
						
						if (checkRequest(request) && evClient[n].events & EPOLLIN)
						{
							ParseRequest	req(request);

							int error = 0;
							if ((error = req.checkProt()) != 0)
							{
								std::cout << "Error: Header" << std::endl;
								handle_request_error(error ,evClient[n], response);
							}
							if (req.getLength() != std::string::npos &&
								req.getLength() > clientPort[evClient[n].data.fd].getClientMaxBodySize())
								handle_request_error(413, evClient[n], response);

							else
							{
								if (req.getMethod() == "GET")
									response.metodGet(evClient[n], req);
								else if (req.getMethod() == "POST")
									response.metodPost(evClient[n], req);
								else if (req.getMethod() == "DELETE")
									response.metodDelete(evClient[n], req);
							}
						}
					}
					close(evClient[n].data.fd);
				}
			}
		
	}
}

std::string	ServerUp::readHttpRequest(int socket)
{
	char		buff[MAX_REQUEST_SIZE + 1];
	std::string	request;
	ssize_t		bytes;
	_reqErr = 0;

	while ((bytes = recv(socket, buff, MAX_REQUEST_SIZE, 0)) > 0)
	{
		buff[bytes] = '\0';
		request.append(buff, bytes);
		_reqErr += bytes;

	}
	_reqErr += bytes;
	return request;
}

void	ServerUp::handle_request_error(int error, epoll_event client, Response response)
{
	if (error > 0)
		response.sendError(error, client);
	if (client.data.fd)
		close(client.data.fd);
}

ServerUp::ServerUp()
{
}

ServerUp::~ServerUp()
{
	vSockets.clear();	
	std::cout << "Bye Bye Server" << std::endl;
}

