#include "../incs/ParseRequest.hpp"

std::string ft_read(std::string filename)
{
	std::ifstream infile(filename.c_str());
	std::string line;
	std::string text;

	if(!infile.is_open())
	{
		//SEND ERROR
		throw MyException("Error 404: Not found");
	}
	while (std::getline(infile, line))
	{
		text.append(line + "\n");
	}
	infile.close();
	return (text);
}

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

	size_t		pos;
	_url = getRoute();
	if ((pos = _url.rfind("?")) != std::string::npos)
	{
		_cgi_body = _url.substr(pos, _url.size());
		_url = _url.substr(0, pos);
	}
	else
		_cgi_body = getBody();

	std::stringstream	strs2(_full_body);
	std::string			line;
	while (std::getline(strs2, line))
	{
		if (line.find(_boundary) != std::string::npos)
		{
			while (line[0] != 13 && std::getline(strs2, line))
			{}
		}
		std::getline(strs2, line);
		if (!(line.find(_boundary) != std::string::npos))
			_rBody += line;
		if (!line.empty())
			_rBody += '\n';
	}
	_rBody.erase(_rBody.size() - 2, _rBody.size() -1);
}

ParseRequest::ParseRequest(const ParseRequest & source)
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
	_full_body = source._full_body;

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
				_full_body += _request[pos];
				pos++;
			}
			if (_boundary.empty())
				_rBody = _full_body;
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

std::string	ParseRequest::getUrl(void)
{
	return _url;
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
std::string	ParseRequest::getBodyCgi(void)
{
	return _cgi_body;
}

std::string	ParseRequest::getBody(void)
{
	return _rBody;
}

std::string	ParseRequest::getFullBody(void)
{
	return _full_body;
}

std::map<std::string, std::string>	ParseRequest::getHeader(void)
{
	return _rHeader;
}
