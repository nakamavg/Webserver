#include "ServerUp.hpp"
bool ServerUp::setupServerSocket(int serverSocket, const sockaddr_in& serverAddress)
{
	int option = 1;
	std::cout << "en setupserverSocket"<<std::endl;
	std::cout << serverSocket<<std::endl;
	std::cout << serverAddress.sin_family<<std::endl;
	std::cout << serverAddress.sin_port<<std::endl;



	
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
			&option, sizeof(option)) < 0)
	{
		perror("");
		close(serverSocket);
		return false;
	}
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		perror("");
		close(serverSocket);
		return false;
	}
	if (listen(serverSocket, SOMAXCONN) < 0)
	{
		perror("");
		return false;
	}
	return true;
}
/*
Inicializa la structura creando un mapa donde la clave es el fd y el valor es la estructura
recorriendo el array de clases de los servidores
*/
void ServerUp::GenStruct(std::map<int, sockaddr_in> *servers , std::vector<int> *sockets)
{
	int i = 0;
	while(i < this->nServers)
	{
		sockaddr_in	serverAddress;
		::bzero(&serverAddress, sizeof(sockaddr_in));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = inet_addr(list[i].get_ip().c_str());
		serverAddress.sin_port = htons(list[i].get_port());
		servers->insert(std::make_pair((*sockets)[i], serverAddress));
		i++;
	}
}

ServerUp::ServerUp(const std::string &ip, size_t port) : ip(ip), port(port)
{

}

size_t ServerUp::getNservers()
{
	return (this->nServers);
}

ServerUp::ServerUp(std::vector<serverlist> &raw) : nServers(0), list(raw)
{
	size_t	nserv;

	std::cout << "pepe" << std::endl;
	nserv = 0;
	std::vector<serverlist>::iterator pailan = list.begin();
	while (pailan != list.end())
	{
		std::cout << (*pailan).get_ip() << std::endl;
		std::cout << (*pailan++).get_port() << std::endl;
		nserv++;
	}
	this->nServers = nserv;
	std::cout << nServers << std::endl;

}
std::vector<int> ServerUp::get_SocketsOfServer()
{
	int	i;
		int serverSocket;

	std::vector<int> sockets;
	i = 1;
	std::cout << this->nServers <<std::endl;

	while (i <= this->nServers)
	{
		if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::cerr << errno << std::endl;
			continue;
		}
		std::cout << serverSocket <<std::endl;
		sockets.push_back(serverSocket);
		i++;
	}
	std::cout << "no salgo de esta funcion" << std::endl;
	return (sockets);
}

void ServerUp::start()
{
	int			serverSocket;
	std::vector<int> vSockets;
	std::map<int, sockaddr_in> se;
	int			option;
	int			epoll_fd;
	epoll_event	evSrv;
	epoll_event	evClient[MAGIC_NUMBER];
	int			fdac;
	int			i;
	int			client_fd;
	char buffer[1024];
	size_t bytesRead;
	vSockets = get_SocketsOfServer();
	
	GenStruct(&se,&vSockets);
	// este es el primer epoll para serverver;
	// vamos a generar la estructura que necesitamos para los eventos de nuevos servers
	epoll_fd = epoll_create(1);
    if (epoll_fd == -1)
    {
        perror("");
        return;
    }
	std::cout << "antes del for" <<std::endl;
	for(std::vector<int>::iterator it = vSockets.begin(); it != vSockets.end(); ++it)
    {
        int serverSocket = *it;
        if (!setupServerSocket(serverSocket, se[serverSocket]))
            continue;
		evSrv.data.fd = serverSocket;
		evSrv.events = EPOLLIN;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket, &evSrv) == -1)
		{
			perror("");
			close(serverSocket);
			continue;
		}
		std::cout << "patata"<<std::endl;
	}
	// estructura para los eventos de conexiones de clientes
	while (42)
	{
		// devuelve el numero de fds que han sido actualizados
				std::cout << "antes del epoll wait" << std::endl;
		fdac = epoll_wait(epoll_fd, evClient, MAGIC_NUMBER, -1);
		if (fdac == -1) {
    perror("epoll_wait failed");
    return;
}
				std::cout << "despues del wait" << std::endl;

		i = -1;
		while (++i < fdac)
		{
			for(auto& serverSocket : vSockets)
    {
      
			if (evClient[i].data.fd == serverSocket)
			{
				std::cout << "antes de accept" << std::endl;
				
				client_fd = accept(serverSocket, NULL, NULL);
				if (client_fd < 0)
				{
					/*
					Los errores EAGAIN y EWOULDBLOCK indican que en ese momento no había conexiones
					disponibles para aceptar (esto es normal en servidores que usan sockets no bloqueantes).
					Si el error es uno de estos,
						no se trata como un fallo crítico.
					*/
					if (errno != EAGAIN && errno != EWOULDBLOCK)
						std::cerr << "Accept error\n";
					continue ;
				}
				std::cout << "despues de accept" << std::endl;
				evClient->events = EPOLLIN | EPOLLET;
				evClient->data.fd = client_fd;
				/*añadimos el fd del accept
				(que es el fd del cliente a el fd del epoll)
				*/
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &evSrv) == -1)
				{
					perror("Client side: ");
					close(client_fd);
				}
				else
				{
					// Aquí entra el manejo del cliente
					// Leer datos del cliente
				std::cout << "antes de leer el mensaje" << std::endl;
					bytesRead = read(client_fd, buffer, sizeof(buffer) - 1);
					if (bytesRead < 0)
					{
						// Manejar error de lectura
						std::cerr << "Error reading from client\n";
					}
					else if (bytesRead == 0)
					{
					std::cout << "el mensaje leido vale 0 bites" << std::endl;
						// El cliente ha cerrado la conexión
						close(client_fd);
					}
					else
					{
						// Procesar los datos recibidos del cliente
						buffer[bytesRead] = '\0';
							// Asegurarse de que el buffer es una cadena de caracteres válida
						std::cout << buffer << "\n";
						std::string response =
						"HTTP/1.1 200 OK\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " "50" "\r\n"
						"\r\n" "<h1>patata</h1><br>patatabaja</br>\r\n";
						send(client_fd, response.c_str(), response.size(), 0);
						close(client_fd);
						if(client_fd > 0)
							close(client_fd);

					}
				}
			}
		}
	}
	}

}

ServerUp::ServerUp(){}

ServerUp::~ServerUp()
{
	std::cout << "manolo";
}
