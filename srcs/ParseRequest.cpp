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
/*
// ParseParseRequest::ParseRequest(std::string _raw_request)
// {
// 	size_t _method_end = _raw_request.find(" ");
// 	size_t _route_lenght = _raw_request.find(" H") - _method_end - 1;
// 	size_t	_line_end = _raw_request.find("\n");
// 	_method = _raw_request.substr(0, _method_end);
// 	_route = _raw_request.substr((_method_end + 1), _route_lenght);
// 	_version = _raw_request.substr();
// 	std::cout << "---->" << _method << "<----" << "    " << "--->" << _route << "<---" << "--->" << _version << "<----"  << std::endl;
// }

ParseParseRequest::ParseRequest(std::string _raw_request, std::vector<ServerConfig> &list) : _conf(NULL)
{
	size_t _method_end = _raw_request.find(" ");
	size_t _route_end = _raw_request.find(" H");
	size_t	_line_end = _raw_request.find(ENDLINE);
	size_t	_rHeaderEnd = _raw_request.find("\r\n\r\n");

	std::string	_hostip;
	std::string _hostport;

	// std::cout << _raw_request << std::endl;
	_rLine = _raw_request.substr(0, _line_end);
	_rHeader = _raw_request.substr(_line_end, (_rHeaderEnd - _line_end));
	_rBody = _raw_request.substr(_rHeaderEnd);

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
		std::cout << it->getHost() << " --- " << _hostip << "\n";

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

ParseParseRequest::~ParseRequest() {}

// ParseParseRequest::ParseRequest(const ParseRequest& source) {(void)source;}

// ParseRequest& ParseParseRequest::operator=(const ParseRequest& source)
// {
//     return(*this);
// }

void ParseParseRequest::ParseLine()
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

void ParseParseRequest::ParseHead()
{

}*/

// void ParseParseRequest::HasChunked()
// {
// 	size_t newline = _rBody.find(END_LINE);
// 	std::string line = _rBody.substr(0, newline);
// 	std::string other = _rBody.substr(newline, _rBody.size() - newline + 4);
// 	size_t charnbr = _rBody.
	
// }

bool	checkRequest(std::string request)
{
	int		sizeBody = request.size() - (request.find("\r\n\r\n") + 4);
	
	if (!request.find("\r\n\r\n"))
		return false;
	if (request.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		if (!request.find("0\r\n\r\n"))
			return false;
		return true;
	}
	else if(request.find("Content-Length") != std::string::npos)
	{
		std::string content = request;
		content.erase(0, content.find("Content-Length") + 16);
		if (!content.find("\r\n"))
			return false;
		content.erase(content.find("\r\n"), content.size());

		if (std::atoi(content.c_str()) > sizeBody)
			return false;
		if (request.find("boundary=") != std::string::npos)
		{
			if (request.find("--"))
				return true;
			return false;
		}
	}
	return true;
}

bool	checkMethod(std::string method, std::vector<std::string> allowed_methods)
{
	for (size_t i = 0; i < allowed_methods.size(); i++)
		if (allowed_methods[i] == method)
			return true;
	return false;
}

ParseRequest::ParseRequest(void)
{}

ParseRequest::~ParseRequest(void)
{}

ParseRequest::ParseRequest(std::string _raw_request)
{
	std::stringstream	strs(_raw_request.c_str());

	strs >> _method;
	strs >> _route;
	strs >> _version;

	_request = _raw_request;
	_length = 0;

	makeCgiBody();

	if (_method == "GET")
		makeGet(strs);
	else if (_method == "POST")
		makePost(strs);
}

ParseRequest::ParseRequest(const ParseRequest &source)
{
	*this = source;
}

ParseRequest	&ParseRequest::operator=(const ParseRequest &source)
{
	_method = source._method;
	_route = source._route;
	_version = source._version;
	_cgi_body = source._cgi_body;
	_boundary = source._boundary;
	_length = source._length;

	_request = source._request;
	_rHeader = source._rHeader;
	_rBody = source._rBody;

	return *this;
}

void	ParseRequest::makeCgiBody(void)
{
	size_t pos = _route.find("?");
	if (pos != std::string::npos)
		_cgi_body = _route.substr(pos + 1);
}

void	ParseRequest::makeGet(std::stringstream & strs)
{
	std::string	token;
	std::string	line;
	std::string	key;

	while (strs >> token)
	{
		if (token[token.size() - 1] == ':')
		{
			if (!key.empty() && !line.empty() && key != token)
			{
				if (line[line.size() - 1] == ' ')
					line.resize(line.size() - 2);
				_rHeader[key] = token;
			}
			token.resize(token.size() - 2);
			key = token;
			line.clear();
		}
		else
		{
			if (!line.empty())
				line += " ";
			line += token;
		}
	}
	if (!line.empty() && !key.empty())
	{
		if (line[line.size() - 1] == ' ')
			line.resize(line.size() - 2);
		_rHeader[key] = token;
	}

}

void	ParseRequest::makePost(std::stringstream &strs)
{
	std::string token;
	std::string	line;
	std::string	key;
	std::string buff;
	std::string full_body;
	size_t pos = _request.find("\r\n\r\n");

	while (strs >> token)
	{
		if (token.find("boundary=") != std::string::npos)
			_boundary = token.substr(token.find("boundary=") + 9);
		else if (token == "Content-Length:")
		{
			if (!key.empty() && !line.empty() && key != token)
			{
				if (line[line.size() - 1] == ' ')
					line.resize(line.size() - 2);
				_rHeader[key] = line;
				line.clear();
			}
			key = token;
			strs >> token;
			_rHeader[key] = token;
			_length = ft_stoi(token);
			key.clear();
		}
		else if (_request.find(token) >= pos - token.length()) 
		{
			pos += 4;
			if (!key.empty() && key != token)
			{
				if (!line.empty())
					line.resize(line.size() - 2);
				if (!line.empty())
					_rHeader[key] = token;
				else
					_rHeader[key] = token;
			}
			size_t pos_rHeader = pos;
			while (pos < _length + pos_rHeader && pos < _request.size())
			{
				full_body += _request[pos];
				pos++;
			}
			if (_boundary.empty())
				_rBody = full_body;
			break;
		}
		else if (token[token.size() - 1] == ':')
		{
			if (!key.empty() && key != token)
			{
				if (line[line.size() - 1] == ' ')
					line.resize(line.size() - 2);
				_rHeader[key] = line;
			}
			token.resize(token.size() - 2);
			key = token;
			line.clear();
		}
		else
		{
			if (!line.empty())
				line += " ";
			line += token;
		}
	}

	std::cout << "Headers:" << std::endl;
}

int		ParseRequest::checkProt(void)
{
	if (_method.size() == 0 || _route.size() == 0 || _version.size() == 0)
		return 400;
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		return 405;
	if (_version != "HTTP/1.1")
		return 505;
	return 0;
}


//GETTERS
std::string	ParseRequest::getRequest(void)
{
	return _request;
}

std::string	ParseRequest::getMethod(void)
{
	return _method;
}

std::string	ParseRequest::getRoute(void)
{
	return _route;
}

std::string	ParseRequest::getProtocol(void)
{
	return _version;
}

size_t	ParseRequest::getLength(void)
{
	return _length;
}

std::string	ParseRequest::getBoundary(void)
{
	return _boundary;
}

std::string	ParseRequest::getBody(void)
{
	return _rBody;
}

std::map<std::string, std::string>	ParseRequest::getHeader(void)
{
	return _rHeader;
}
