#include "../incs/ServerUp.hpp"
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
		perror("fnclt");
		return (0);
	}
	if (fcntl(sock, F_SETFL, flag | O_NONBLOCK) < 0)
	{
		perror("fnctl");
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
		perror("");
		close(serverSocket);
		return (false);
	}
	if (bind(serverSocket, (struct sockaddr *)&serverAddress,
			sizeof(serverAddress)) < 0)
	{
		perror("");
		close(serverSocket);
		return (false);
	}
	if (listen(serverSocket, SOMAXCONN) < 0)
	{
		perror("");
		return (false);
	}
	return (true);
}
/*
Inicializa la structura creando un mapa donde la clave es el fd y el valor es la estructura
recorriendo el array de clases de los servidores
*/
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
		serverAddress.sin_addr.s_addr = inet_addr(list[i].getHost().c_str());
		serverAddress.sin_port = htons(list[i].getPort());
		servers->insert(std::make_pair((*sockets)[i], serverAddress));
		serverPort->insert(std::make_pair((*sockets)[i], list[i]));

		i++;
	}
}
void ServerUp::newConect(int serverfd, int fdEpoll,std::map<int,ServerConfig> &serverPort,std::map<int, ServerConfig> &clientPort)
{
	epoll_event ev;
	sockaddr in_addr;
	socklen_t in_addr_len = sizeof(in_addr);
	int newfd;
	std:: cout<< "en newconnect antes de accept " << serverfd << std::endl;
	if ((newfd = accept(serverfd, (struct sockaddr *)&in_addr, &in_addr_len)) < 0)
	{
		if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
		{
				std::cout << "we already processed all incoming connections" << std::endl;
		}
		else
		{
			perror("accept()");
		}
	}
		setsocknonblock(newfd);
	::bzero(&ev,sizeof(ev));
		clientPort[newfd] = serverPort[serverfd];
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd= newfd;
	if(epoll_ctl(fdEpoll,EPOLL_CTL_ADD,newfd, &ev)< 0)
		perror("epoll control");

}

ServerUp::ServerUp(const std::string &ip, size_t port) : ip(ip), port(port)
{
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
		*pailan++;
		nserv++;
	}
	this->nServers = nserv;
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
	int			epoll_fd;
	epoll_event	evClient[MAX_EVENTS];
	int			fdac;
	//char		buffer[99999];
	std::string response;
	epoll_event	ev;
	std::string html =ft_read("html/index.html");


	std::map<int, sockaddr_in> se;
	std::map<int, ServerConfig> serverPort;
	std::map<int, ServerConfig> clientPort;

	vSockets = get_SocketsOfServer();
	GenStruct(&se, &vSockets, &serverPort);
	// este es el primer epoll para serverver;
	// vamos a generar la estructura que necesitamos para los eventos de nuevos servers
	epoll_fd = epoll_create(MAX_EVENTS);
	
	if (epoll_fd == -1)
	{
		perror("");
		return ;
	}
	for (std::vector<int>::iterator it = vSockets.begin(); it != vSockets.end(); ++it)
	{
		if (!setupServerSocket(*it, se[*it]))
			continue ;
		::bzero(&ev,sizeof(ev));	
		ev.events = EPOLLIN | EPOLLOUT; 
		ev.data.fd = *it;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, *it, &ev) == -1)
		{
			perror("");
			close(*it);
			continue ;
		}
	}
	// estructura para los eventos de conexiones de clientes
	while (42)
	{
		// devuelve el numero de fds que han sido actualizados
		fdac = epoll_wait(epoll_fd, evClient, MAX_EVENTS, -1);
		if (fdac == -1)
		{
			perror("epoll_wait failed");
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
					
					std::string	request;

					request = readHttpRequest(evClient[n].data.fd);

					if (evClient[n].events & EPOLLOUT)
					{
						if (_reqErr < 0)
						{
							std::cout << "Error: Recv failed" << std::endl;
							//handle_request_error(500, _client[i], _read_set, i);
						}
						else if (_reqErr == 0)
						{
							std::cout << "Connection is closed" << std::endl;
							//handle_request_error(0, _client[i], _read_set, i);
						}
						//posible error si la request es erronea
						if (checkRequest(request) && evClient[n].events & EPOLLIN)
						{
							ParseRequest	req(request);
							Response		response(clientPort[evClient[n].data.fd]);

							int error = 0;
							if ((error = req.checkProt()) != 0)
							{
								std::cout << "Error: Header" << std::endl;
								//handle_request_error(ret, _client[i], _read_set, i);
							}
							if (req.getLength() != std::string::npos &&
								req.getLength() > clientPort[evClient[n].data.fd].getClientMaxBodySize())
							{
								// Uncomment and handle the error as needed
								// handle_request_error(413, _client[i], _read_set, i);
							}
							
							//allow metod general

							//else
								//if ()
									//redir
							//comprobar q es cgi
							/*if(true)
							{
								Cgi a("cgi/a.out","manolo pepe");
								a.handlerCgi();
								std::cout << a.get_output()<< "\n";
								//response=Response(a.get_output()).get_web();

							}*/
							//else
							//{
								if (req.getMethod() == "GET")
									response.metodGet(evClient[n], req);//falta location
								else if (req.getMethod() == "POST")
									response.metodPost(evClient[n], req);
								else if (req.getMethod() == "DELETE")
									response.metodDelete(evClient[n], req);
							//}
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
	size_t		bytes;
	_reqErr = 0;

	while ((bytes = recv(socket, buff, MAX_REQUEST_SIZE, 0)) > 0)
	{
		request += buff;
		_reqErr += bytes;
		if (request.find("\r\n\r\n") != std::string::npos)
			return request;
	}
	_reqErr += bytes;
	return "";
}


ServerUp::ServerUp()
{
}

ServerUp::~ServerUp()
{
	std::cout << "Bye Bye Server" << std::endl;
}

