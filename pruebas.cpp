/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pruebas.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anurtiag <anurtiag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 13:06:03 by anurtiag          #+#    #+#             */
/*   Updated: 2024/10/03 12:14:44 by anurtiag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fstream>
#include <iomanip>
#include <exception>
#include <unistd.h>
#include <stdlib.h>

#define ENDLINE "\r\n"

// bool	ft_logs(std::string request)
// {
// 	std::ofstream outfile("logs");
// 	size_t pos;
// 	if (!outfile.is_open())
// 	{
// 		std::cerr << "outfile could not be opened :(" << std::endl;
// 		return(false);
// 	}
// 	outfile << text;
// 	return(true);



// bool GetParsing(std::string request)
// {
//     size_t mpos = request.find("GET");
//     size_t spos = request.find("HTTP/1.1");
    
//     if(mpos != std::string::npos && spos != std::string::npos)
//     {
//         std::cout << "valid method \n";
//         return(true);
//     }
//     else
//         std::cout << "invalid method\n";
//     return(false);
// }

// bool ParseFLine(std::string fline)
// {
//     size_t mpos = fline.find("GET");
//     size_t spos = fline.find("HTTP/1.1");
//     if(mpos == std::string::npos)
//     {
//         std::cerr << "invalid method\n";
//         return (false);
//     }
//     else if(spos == std::string::npos)
//     {
//         std::cerr << "invalid standard\n";
//         return (false);
//     }
// }

// bool ParseSLine(std::string fline)
// {
    
// }

// bool GetParsing(std::string request)// de esto se podria hacer clase padre, habra que mirarlo
// {
//     if(request.empty() == true)
//         return(true);
//     size_t npos = request.find("\n");
//     std::string fline = request.substr(0, npos + 1);
//     request.erase(0, npos + 1);
//     npos = request.find("\n");
//     std::string sline = request.substr(0, npos + 1);
//     std::cout << fline << sline << "garbanzo" << std::endl;
//     return(false);
// }

std::string ft_read(std::string filename)
{
	std::ifstream infile(filename.c_str());
	std::string line;
	std::string text;

	if(!infile.is_open())
	{
		std::cerr << "infile could not be opened" << std::endl;
		return("");
	}
	while (std::getline(infile, line))
	{
		text.append(line + "\n");
	}
	infile.close();
	return (text);
}

//funcion que genera sockets
int ft_sockets()
{
    int clientfd[100];
    char rbuffer[1000000];
    int option = 1;
    std::string wbuffer;
    std::string garbanzo = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\n\r\n" "6\r\nPailan"  "4\r\nPuta\r\n" "0\r\n";
    std::string html = ft_read("./html/index1.html");
    std::string html1 = ft_read("./html/holamundo.html");
    std::string length = std::to_string(html.size());
    std::string response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: " + length + "\r\n"
    "\r\n" + html.c_str();
    std::string response1 =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: " + html1 + "\r\n"
    "\r\n" + html1.c_str();
    std::string chunkedresponse1 =
    std::string("HTTP/1.1 200 OK\r\n") +
    "Transfer-Encoding: chunked\r\n"
    "Content-Type: text/html\r\n"
    "\r\n" + "581" + ENDLINE +
    html1.substr(0, 580) + ENDLINE +
    "581" + ENDLINE +
    html1.substr(581, 1162) + ENDLINE;
    std::string chunked1 =
    std::string("HTTP/1.1 200 OK\r\n") +
    "Transfer-Encoding: chunked\r\n"
    "Content-Type: text/html\r\n"
    "\r\n" + "581" + ENDLINE +
    html1.substr(0, 580) + ENDLINE;
    std::string chunked2 = std::string("581") + ENDLINE +
    html1.substr(581, 1162) + ENDLINE;
    int sockets = socket(AF_INET, SOCK_STREAM, 0);
    if (sockets < 0)
    {
        std::cout << "fallo en la creacion del socket\n";
        _exit(1);
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;//esto es que el socket va a usar ipv4
    address.sin_addr.s_addr = inet_addr("127.0.0.1");//ip en el que el socket va a escuchar
    // address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);//puerto de la ip en el que va a escuchar
    setsockopt(sockets, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
    if (bind(sockets, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Error en bind" << std::endl;
        close(sockets);
        _exit(1);
    }
    if (listen(sockets, SOMAXCONN) < 0)
    {
        std::cerr << "algo falla en la escucha\n";
        _exit(1);
    }

    //parte del epoll

    int epoll = epoll_create1(EPOLL_CLOEXEC);
    struct epoll_event event;
    struct epoll_event events[100];
    struct epoll_event client;

    
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = sockets;
    if(epoll_ctl(epoll, EPOLL_CTL_ADD, sockets, &event) < 0)
    {
        // std::cerr << "bobo que eres tontisimo\n";
    }
    while(1)
    {
        int num_events = epoll_wait(epoll, events, 100, 0);
        if (num_events == -1)
            break ;
        for(int i = 0; i < num_events; i++)
        {
            // events[i].events = EPOLLIN | EPOLLOUT;
            if(events[i].data.fd == sockets)
            {
                clientfd[i] = accept(sockets, NULL, NULL);
                // std::cout << "hemos aceptado el socket\n";
                if(clientfd[i] > 0)
                {
                    client.data.fd = clientfd[i];
                    client.events = EPOLLIN;
                    epoll_ctl(epoll, EPOLL_CTL_ADD, clientfd[i], &client);
                }
            }
            if(events[i].events & EPOLLIN)//hay un evento de escritura
            {
                // std::cout << "entramos al modo lectura\n";
                int nRead = read(clientfd[i], rbuffer, 999999);
                rbuffer[nRead] = '\0';
                // GetParsing(rbuffer);
                std::string b = rbuffer;
                if(nRead < 1)
                    break ;
                std::cout << rbuffer << std::endl;
                if(b.find("/ ") != std::string::npos)
                {
                    send(clientfd[i], response.c_str(), response.length(), 0);
                    close(clientfd[i]);
                }
                else if(b.find("holamundo.html") != std::string::npos)
                {
                    // send(clientfd[i], chunkedresponse1.c_str(), chunkedresponse1.size(), 0);
                    send(clientfd[i], chunked1.c_str(), chunked1.size(), 0);
                    usleep(50);
                    send(clientfd[i], chunked2.c_str(), chunked2.size(), 0);
                    close(clientfd[i]);
                }
                // else
                //     send(clientfd[i], "garbanzo", 8, 0);
                // send(clientfd[i], garbanzo.c_str(), garbanzo.length(), 0);
            }
        }
    }
    return(sockets);
}



int main(void)
{
    int socket = ft_sockets();
    socket++;
    std::cout << "\n\n\nno salimos por aqui\n";
    return(0);
}