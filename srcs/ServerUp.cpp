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
	//std::cout << "en setupserverSocket" << std::endl;
	//std::cout << serverSocket << std::endl;
	//std::cout << serverAddress.sin_family << std::endl;
	//std::cout << serverAddress.sin_port << std::endl;
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
	std::vector<int> *sockets)
{
	int			i;
	sockaddr_in	serverAddress;

	i = 0;
	while (i < this->nServers)
	{
		::bzero(&serverAddress, sizeof(sockaddr_in));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = inet_addr(list[i].getHost().c_str());
		serverAddress.sin_port = htons(list[i].getPort());
		servers->insert(std::make_pair((*sockets)[i], serverAddress));
		i++;
	}
}
void ServerUp::newConect(int serverfd, int fdEpoll)
{
	epoll_event ev;
	sockaddr in_addr;
	socklen_t in_addr_len = sizeof(in_addr);
	int newfd;
	std:: cout<< "en newconnect antes de accept " << serverfd << std::endl;
	if ((newfd = accept(serverfd, (struct sockaddr *)&in_addr, &in_addr_len)) < 0)
	{
		exit(6);
		if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
		{
				//std::cout << "we already processed all incoming connections" << std::endl;
		}
		else
		{
			perror("accept()");
		}
	}
		setsocknonblock(newfd);
	::bzero(&ev,sizeof(ev));
	ev.events = EPOLLIN;
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

std::vector<ServerConfig>& ServerUp::GetList()
{
	return(this->list);
}

ServerUp::ServerUp(const std::vector<ServerConfig> &raw) : nServers(0), list(raw)
{
	size_t	nserv;

	//std::cout << "pepe" << std::endl;
	nserv = 0;
	std::vector<ServerConfig>::iterator pailan = list.begin();
	while (pailan != list.end())
	{
		std::cout << (*pailan).getHost() << std::endl;
		std::cout << (*pailan++).getPort() << std::endl;
		std::cout << std::endl;
		nserv++;
	}
	this->nServers = nserv;
	//std::cout << nServers << std::endl;
}
std::vector<int> ServerUp::get_SocketsOfServer()
{
	int	i;
	int	serverSocket;

	std::vector<int> sockets;
	i = 1;
	//std::cout << this->nServers << std::endl;
	while (i <= this->nServers)
	{
		if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::cerr << errno << std::endl;
			continue ;
		} 
		//std::cout << serverSocket << std::endl;
		sockets.push_back(serverSocket);
		i++;
	}
	//std::cout << "no salgo de esta funcion" << std::endl;
	return (sockets);
}

void ServerUp::start()
{
	int			serverSocket;
	int			option;
	int			epoll_fd;
	epoll_event	evClient[MAX_EVENTS];
	int			fdac;
	int			i;
	int			client_fd;
	char		buffer[99999];
	size_t		bytesRead;
	epoll_event	ev;
	epoll_event	clients;
	std::string html =ft_read("html/post.html");


	std::map<int, sockaddr_in> se;
	vSockets = get_SocketsOfServer();
	GenStruct(&se, &vSockets);
	// este es el primer epoll para serverver;
	// vamos a generar la estructura que necesitamos para los eventos de nuevos servers
	epoll_fd = epoll_create(MAX_EVENTS);
	
	if (epoll_fd == -1)
	{
		perror("");
		return ;
	}
	//std::cout << "antes del for" << std::endl;
	for (std::vector<int>::iterator it = vSockets.begin(); it != vSockets.end(); ++it)
	{
		if (!setupServerSocket(*it, se[*it]))
			continue ;
		::bzero(&ev,sizeof(ev));	
		ev.events = EPOLLIN;
		ev.data.fd = *it;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, *it, &ev) ==
			-1)
		{
			perror("");
			close(*it);
			continue ;
		}
		//std::cout << "patata" << std::endl;
	}
	// estructura para los eventos de conexiones de clientes
	while (42)
	{
		// devuelve el numero de fds que han sido actualizados
		//std::cout << "antes del epoll wait" << std::endl;
		fdac = epoll_wait(epoll_fd, evClient, MAX_EVENTS, -1);
		if (fdac == -1)
		{
			perror("epoll_wait failed");
			return ;
		}
		//std::cout << "despues del wait" << std::endl;
			for(int n = 0;n < fdac; n++)
			{
				if(int fdconnect = checkfd(evClient[n].data.fd))
				{	
					//std::cout << fdconnect << std::endl;
					 newConect(fdconnect,epoll_fd);
					 //std::cout << "despues del accept"<<std::endl;
					 break;
				}
				if(evClient[n].events & EPOLLIN)
				{
                int nRead = read(evClient[n].data.fd, buffer, 999999);
                buffer[nRead] = '\0';
                std::string b = buffer;
				ParseRequest request(b, this->GetList());
				//std::cout << b << std::endl;
				//std::cout << html << std::endl;
						// Procesar los datos recibidos del cliente
						std::string response =
					"HTTP/1.1 200 OK\r\n"
					"Content-Type: text/html\r\n"
					"Content-Length: " + std::to_string(html.size()) + "\r\n"
					"\r\n" + html.c_str() ;
						send(evClient[n].data.fd, response.c_str(), response.size(), 0);
						close(evClient[n].data.fd);
				}
			}
		
	}
}

ServerUp::ServerUp()
{
}

ServerUp::~ServerUp()
{
	//std::cout << "manolo";
}

