/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anurtiag <anurtiag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:12:44 by anurtiag          #+#    #+#             */
/*   Updated: 2024/10/09 13:50:45 by anurtiag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ParseRequest.hpp"

std::string ft_read(std::string filename)
{
	std::ifstream infile(filename.c_str());
	std::string line;
	std::string text;

	if(!infile.is_open())
	{
		throw MyException("Error 404: Not found");
	}
	while (std::getline(infile, line))
	{
		text.append(line + "\n");
	}
	infile.close();
	return (text);
}

// ParseRequest::ParseRequest(std::string _raw_request)
// {
// 	size_t _method_end = _raw_request.find(" ");
// 	size_t _route_lenght = _raw_request.find(" H") - _method_end - 1;
// 	size_t	_line_end = _raw_request.find("\n");
// 	_method = _raw_request.substr(0, _method_end);
// 	_route = _raw_request.substr((_method_end + 1), _route_lenght);
// 	_version = _raw_request.substr();
// 	std::cout << "---->" << _method << "<----" << "    " << "--->" << _route << "<---" << "--->" << _version << "<----"  << std::endl;
// }

ParseRequest::ParseRequest(std::string _raw_request, std::vector<ServerConfig> &list) : _conf(NULL)
{
	size_t _method_end = _raw_request.find(" ");
	size_t _route_end = _raw_request.find(" H");
	size_t	_line_end = _raw_request.find(ENDLINE);
	size_t	_headerEnd = _raw_request.find("\r\n\r\n");
	std::string	_hostip;
	std::string _hostport;
	// std::cout << _raw_request << std::endl;
	_rLine = _raw_request.substr(0, _line_end);
	_rHeader = _raw_request.substr(_line_end, (_headerEnd - _line_end));
	_rBody = _raw_request.substr(_headerEnd);
	size_t _host = _rHeader.find("Host: ");
	_host += 6;
	size_t _hostend = _host;
	for(; _rHeader[_hostend] != ':'; _hostend++)
		;
	_hostip = _rHeader.substr(_host, _hostend - _host);
	_host = _hostend + 1;
	for(; _rHeader[_hostend] != '\r'; _hostend++)
		;
	_hostport = _rHeader.substr(_host, (_hostend - _host));
	std::cout << "IP REQUEST " << _hostip << "PUERTO REQUEST " << _hostport << std::endl;
	for(std::vector<ServerConfig>::iterator it = list.begin(); it != list.end(); it++)
	{
		if((*it).getHost() == _hostip && (*it).getPort() == std::atoi(_hostport.c_str()))
		{
				std::cout << "FUNTXIONAAAA\n";
				_conf = &(*it);
		}
		else
			std::cout << "HOST " << (*it).getHost() << "PUERTO " << (*it).getPort() << std::endl;
	}
	std::cout << "ya en el conf " << _conf->getHost() << _conf->getPort() << std::endl;
	// _method = _raw_request.substr(0, _method_end);
	// _route = _raw_request.substr((_method_end + 1), _route_end - _method_end - 1);
	// _version = _raw_request.substr((_route_end + 1), _line_end - _route_end - 1);
	// std::cout << "LINEA DE PETISION" << std::endl << _rLine << std::endl << "CABECERA DE PETISION" << _rHeader << std::endl << "CUERPO DE PETISION" << _rBody << _rBody.size() << std::endl;
	// if(_rBody == "\r\n\r\n")
	// 	std::cout << "PATATAAAAAAAAAA\n";
	// std::cout << "---->" << _method << "<----" << std::endl << "--->" << _route << "<---" << std::endl << "--->" << _version << "<----"  << std::endl;
	// if(_method != "GET" && _method == "POST" && _method == "DELETE")
    //     throw MyException("Error 501: Not implemented");
	// if(_version != "HTTP/1.1")
    //     throw MyException("Error 505: HTTP Version Not Supported");
	this->ParseLine();
}

ParseRequest::~ParseRequest() {}

// ParseRequest::ParseRequest(const ParseRequest& source) {(void)source;}

// ParseRequest& ParseRequest::operator=(const ParseRequest& source)
// {
//     return(*this);
// }

void ParseRequest::ParseLine()
{
	std::string other;
	size_t _method_end = _rLine.find(" ");
	_method = _rLine.substr(0, _method_end);
	other = _rLine.substr(_method_end + 1);
	// std::cout << "METODO " << _method << std::endl << other << std::endl;
	size_t _route_end = other.find(" ");
	_route = other.substr(0, _route_end);
	_version = other.substr(_route_end + 1);
	std::cout << "METODO " << _method << std::endl << "RUTA " << _route << std::endl << "VERSION " << _version << std::endl;
	if(_method != "GET" && _method == "POST" && _method == "DELETE")
    	throw MyException("Error 501: Not implemented");
	const std::map<std::string, Locations>& map = _conf->getLocations();
	for(std::map<std::string, Locations>::const_iterator it = map.begin(); it != map.end(); it++)
		std::cout << "EL STRING DEL MAPA ES: " << it->first << std::endl;

}

void ParseRequest::ParseHead()
{

}

// void ParseRequest::HasChunked()
// {
// 	size_t newline = _rBody.find(END_LINE);
// 	std::string line = _rBody.substr(0, newline);
// 	std::string other = _rBody.substr(newline, _rBody.size() - newline + 4);
// 	size_t charnbr = _rBody.
	
// }