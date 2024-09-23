#include "ServerUp.hpp"

void ServerUp::GenStruct(sockaddr_in *serverAddress)
{
	serverAddress->sin_family = AF_INET;
	serverAddress->sin_addr.s_addr = inet_addr(ip.c_str());
	serverAddress->sin_port = htons(port);
}
ServerUp::ServerUp(const std::string &ip, size_t port) : ip(ip), port(port)
{
}

void ServerUp::start()
{
	int			serverSocket;
	sockaddr_in	serverAddress;
	int			option;
	int			epoll_fd;
	epoll_event	evSrv;
	epoll_event	evClient[MAGIC_NUMBER];
	int			fdac;
	int			i;
	int			client_fd;

	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << errno << std::endl;
		return ;
	}
	::bzero(&serverAddress, sizeof(sockaddr_in));
	GenStruct(&serverAddress);
	option = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
			&option, sizeof(option)) < 0)
	{
		perror("");
		close(serverSocket);
	}
	if (bind(serverSocket, (struct sockaddr *)&serverAddress,
			sizeof(serverAddress)) == -1)
	{
		perror("");
		return ;
	}
	/*
		La macro SOMAXCONN esta hardcodeada a 4096 conexiones maximas desde una version de linux en adelante
	*/
	if (listen(serverSocket, SOMAXCONN) < 0)
		perror("");
	// la flag nos cierra el fd despues de exec
	epoll_fd = epoll_create(1);
	// este es el primer epoll para serverver;
	// vamos a generar la estructura que necesitamos para los eventos de nuevos servers
	evSrv.data.fd = serverSocket;
	evSrv.events = EPOLLIN ;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket, &evSrv) == -1)
	{
		perror("");
		close(epoll_fd);
		return ;
	}
	// estructura para los eventos de conexiones de clientes
	while (42)
	{
		// devuelve el numero de fds que han sido actualizados
		fdac = epoll_wait(epoll_fd, evClient, MAGIC_NUMBER, -1);
		i = -1;
		while (++i < fdac)
		{
			if (evClient[i].data.fd == serverSocket)
			{
				/*
					si el fd del evento es igual al del socket
					significa que hay una nueva conexion
				*/
				client_fd = accept(serverSocket, NULL, NULL);
				if (client_fd < 0)
				{
					/*
					Los errores EAGAIN y EWOULDBLOCK indican que en ese momento no había conexiones 
					disponibles para aceptar (esto es normal en servidores que usan sockets no bloqueantes). 
					Si el error es uno de estos, no se trata como un fallo crítico.
					*/
					if (errno != EAGAIN && errno != EWOULDBLOCK)
						std::cerr << "Accept error\n";
					continue ;
				}
				evClient->events = EPOLLIN | EPOLLET;
        		evClient->data.fd = client_fd;
				/*añadimos el fd del accept 
				(que es el fd del cliente a el fd del epoll)
				*/
				if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &evSrv) == -1)
				{
					perror("Client side: ");
					close(client_fd);
				}
				else
				{		
						// Aquí entra el manejo del cliente
						char buffer[1024];
						ssize_t bytesRead;

						// Leer datos del cliente
						bytesRead = read(client_fd, buffer, sizeof(buffer) - 1);
						if (bytesRead < 0)
						{
							// Manejar error de lectura
							std::cerr << "Error reading from client\n";
						}
						else if (bytesRead == 0)
						{
							// El cliente ha cerrado la conexión
							close(client_fd);
						}
						else
						{
							// Procesar los datos recibidos del cliente
							buffer[bytesRead] = '\0'; // Asegurarse de que el buffer es una cadena de caracteres válida
							std::cout << buffer << "\n";
						}
					}
				}

			}
		}
	}

ServerUp::ServerUp(){};

ServerUp::~ServerUp()
{
	std::cout << "manolo";
}
