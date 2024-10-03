/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anurtiag <anurtiag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:12:44 by anurtiag          #+#    #+#             */
/*   Updated: 2024/10/02 11:27:57 by anurtiag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseRequest.hpp"

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

ParseRequest::ParseRequest(std::string _raw_request)
{
	size_t _method_end = _raw_request.find(" ");
	size_t _route_end = _raw_request.find(" H");
	size_t	_line_end = _raw_request.find("\n");
	_method = _raw_request.substr(0, _method_end);
	_route = _raw_request.substr((_method_end + 1), _route_end - _method_end - 1);
	_version = _raw_request.substr((_route_end + 1), _line_end - _route_end - 1);
	if(_method != "GET" && _method == "POST" && _method == "DELETE")
        throw MyException("Error 501: Not implemented");
	if(_version != "HTTP/1.1")
        throw MyException("Error 505: HTTP Version Not Supported");
	std::cout << "---->" << _method << "<----" << "--->" << _route << "<---" << "--->" << _version << "<----"  << std::endl;
}

ParseRequest::~ParseRequest() {}

ParseRequest::ParseRequest(const ParseRequest& source) {}

ParseRequest& ParseRequest::operator=(const ParseRequest& source)
{
    return(*this);
}

void ParseRequest::RequestMethod()
{

}

// void ParseRequest::ParseHead()
// {

// }
