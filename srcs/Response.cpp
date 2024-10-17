#include "../incs/Response.hpp"

/*Response::Response (std::string &html)
{
    std::stringstream ss;
    ss << html.size();

    this->web =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + ss.str() + "\r\n"
        "\r\n" + html.c_str() ;
}
Response::~Response(){};
std::string Response::get_web()
{
	return this->web;
}*/

Response::Response(void)
{}

Response::Response(ServerConfig * conf)
{
	setErrors();
    _default_error = "HTTP/1.1 404 Not Found\n";
	_default_error += "Content-Type: text/plain\n";
	_default_error += "Content-Length: 15\n\n";
	_default_error += "404 Not Found\n";
    setFileTypes();
	_conf = conf;
}

Response::Response(const Response & source)
{
	*this = source;
}

Response	&Response::operator=(const Response & source)
{
	_default_error = source._default_error;
	_errors = source._errors;
	_fileTypes = source._fileTypes;
	_conf = source._conf;
	return *this;
}

Response::~Response(void)
{}

void	Response::sendPage(std::string page, epoll_event & client, std::string request, int error)
{
	std::cout << "Show Page: " << page << std::endl;

	if (page.empty())
	{
		std::string msg = "HTTP/1.1 ";
		msg += _errors[error];
		msg += "\n\n";
		size_t i;
		if ((i = send(client.data.fd, msg.c_str(), msg.size(), 0)) <= 0)
			sendError(500, client);
		return ;
	}
	else
	{
		if (request.find("Transfer-Encoding: chunked") != std::string::npos)
		{
			sendChuncked(page, client, error);
			return ;
		}
		std::ifstream fd(page.c_str());
	
		if (!fd.is_open())
		{
			sendError(404, client);
			return;
		}

		fd.seekg(0, std::ios::end);
		size_t size = fd.tellg();
		fd.seekg(0, std::ios::beg);

		std::string type = findType(page);

		std::string msg = "HTTP/1.1 ";
		msg += _errors[error];
		msg += "\nContent-Type: ";
		msg += type;
		msg += "\nContent-Length: ";
		msg += ft_size_to_str(size);
		msg += "\n\n";

		size_t i;
		if ((i = send(client.data.fd, msg.c_str(), msg.size(), 0)) <= 0) {
			sendError(500, client);
			return;
		}

		char buffer[1024];
		while (size > 0)
		{
			int bytes_to_read = std::min(static_cast<size_t>(1024), size);
			fd.read(buffer, bytes_to_read);
			if (fd.eof() && fd.fail())
			{
				sendError(500, client);
				return;
			}
			size_t n;
			if ((n = send(client.data.fd, buffer, fd.gcount(), MSG_NOSIGNAL)) <= 0)
			{
				sendError(500, client);
				return;
			}
			size -= n;
		}
		fd.close();
	}
}

void	Response::sendError(int error, epoll_event & client)
{
	std::map<std::string, std::string> errorPages;
	int fd;

	//cojer las error pages del conf
	//errorPages = _server[client.getServ()].getErrorPages();
	if (!errorPages[ft_size_to_str(error)].empty() && errorPages[ft_size_to_str(error)].size() <= 0)
	{
		//raiz del servidor
		fd = open(("html" + '/' + errorPages[ft_size_to_str(error)]).c_str(), O_RDONLY);
		if (fd < 0)
		{
			std::cout << "Error: Error pages failed -> " << error << std::endl;
			errorPages.erase(errorPages.find(ft_size_to_str(error)));
			sendError(error, client);
			return;
		}
		close(fd);
		//raiz del servidor
		sendPage("html" + '/' + errorPages[ft_size_to_str(error)], client, "", 200);
	}
	else
	{
		if (!_errors[error].empty())
		{
			std::cout << "Error Page: " << _errors[error] << std::endl;
			int i;
			std::string msg = "HTTP/1.1 ";
			msg += _errors[error] + "\n";
			msg += "Content-Type: text/plain\n";
			msg += "Content-Length: ";
			msg += ft_size_to_str(_errors[error].length() + 1);
			msg += "\n\n";
			msg += _errors[error] + "\n";

			i = send(client.data.fd, msg.c_str(), msg.length(), 0);
			if (i < 0)
				std::cout << "Client disconnected" << std::endl;
			else if (i == 0)
				std::cout << "0 byte passed to server" << std::endl;
		}
	}
}

void	Response::sendChuncked(std::string page, epoll_event & client, int error)
{
	std::cout << "Show Chuncked Page: " << page << std::endl;

	std::ifstream	fd(page.c_str());
	if (!fd.is_open())
	{
		sendError(404, client);
		return;
	}

	std::string type = findType(page);
	std::string header = "HTTP/1.1 ";
	header += _errors[error];
	header += "\r\nContent-Type: ";
	header += type;
	header += "\r\nTransfer-Encoding: chunked";
	header += "\r\n\r\n";

	size_t i;
	if ((i = send(client.data.fd, header.c_str(), header.size(), 0)) <= 0)
	{
		sendError(500, client);
		fd.close();
		return;
	}

	char	buff[1024];

	while(!fd.eof())
	{
		fd.read(buff, sizeof(buff));

		size_t	size = fd.gcount();
		if (size > 0)
		{
			std::stringstream	ss;
			ss << std::hex << size;
			std::string msg = ss.str() + "\r\n";

			msg = msg + buff + "\r\n";
			if ((i = send(client.data.fd, msg.c_str(), msg.size(), 0)) <= 0)
			{
				sendError(500, client);
				fd.close();
				return;
			}
		}
	}
	if ((i = send(client.data.fd, "0\r\n\r\n", 5, 0)) <= 0)
	{
		sendError(500, client);
		return;
	}
	fd.close();
}

bool	Response::writePost(std::string path, epoll_event & client, std::string str)
{
	int	fd = open(path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
	{
		sendError(500, client);
		return false;
	}

	if (!write(fd, str.c_str(), str.length()))
	{
		sendError(500, client);
		close(fd);
		return false;
	}
	close(fd);
	return true;
}

//METHODS
void	Response::metodGet(epoll_event & client, ParseRequest & request)
{
	std::cout << "Get Method\n";

	std::string	url;
	url = request.getRoute();

	if (url.size() >= 64)
	{
		sendError(414, client);
		return ;
	}

	//raiz del server
	const std::map<std::string, Locations>& map = _conf->getLocations();
	for(std::map<std::string, Locations>::const_iterator it = map.begin(); it != map.end(); it++)
		std::cout << "EL STRING DEL MAPA ES: " << it->first << std::endl;

	struct Locations *loc = NULL;
	loc = &_conf->getLocations().find("Default")->second;
	if (loc == NULL)
	{
		std::cout << "123\n";
		loc = &_conf->getLocations()["Default"];
	}
	std::string	path = loc->path;
	std::cout << "123 - " << path << "\n";

	/*if (location && !location->getIndex().empty() && checkIndex(path, location->getIndex()))
	{
		sendPage(path + '/' + location->getIndex(), client, request.getRequest(), 200);
		return ;
	}*/

	struct stat	stat_path;
	int	fd = open(path.c_str(), O_RDONLY);
	stat(path.c_str(), &stat_path);

	if (fd <= 0)
	{
		sendError(404, client);
		return ;
	}
	if (S_ISDIR(stat_path.st_mode))
	{	
		//index
		if (checkIndex(path, "index.html")/* && !location*/)
			sendPage(path + '/' + "index.html", client, request.getRequest(), 200);
		/*else if (_server[client.getServ()].getListing() && location && location->getListing())
			listing(client, url, path);*/
		else
			sendError(404, client);
	}
	else
		sendPage(path, client, request.getRequest(), 200);
	close(fd);
}

void	Response::metodPost(epoll_event & client, ParseRequest & request)
{
	std::cout << "Post Method\n";

	//raiz del servidor
	std::string	path = "html/" + request.getRoute();
	struct stat	stat_path;
	lstat(path.c_str(), &stat_path);

	if (S_ISDIR(stat_path.st_mode))
	{
		std::string	body = request.getFullBody();
		std::string	file;
	
		if (!(request.getHeader().empty() && request.getBoundary().empty()))
		{

			std::cout << "Post in directory: " << std::endl;

			size_t start = 0;
			while (true)
			{
				start = body.find("name=\"", start);
				if (start == std::string::npos)
					break;
				start += 6;
				size_t end = body.find("\"", start);
				if (end == std::string::npos)
					break;
				std::string name = body.substr(start, end - start);
				std::cout << "+ " + name << std::endl;

				start = body.find("\r\n\r\n", end);
				if (start == std::string::npos)
					break;
				start += 4;
				end = body.find(request.getBoundary(), start);
				if (end == std::string::npos)
					break;

				file = body.substr(start, end - start - 4);

				if (!writePost(request.getRoute() + "/" + name, client, file))
					break;

				if (body[end + request.getBoundary().size()] == '-')
					break;
			}
		}
		else
		{
			sendError(400, client);
			return ;
		}
	}
	else
	{

		std::cout << "POST IN FILE\n";

		if (!writePost(path, client, request.getFullBody()))
			return ;
	}
	if (request.getLength() == 0)
		sendPage("", client, request.getRequest(), 204);
	else
		sendPage("", client, request.getRequest(), 201);
}

void	Response::metodDelete(epoll_event & client, ParseRequest & request)
{
	std::cout << "Delete Method\n";

	//raiz del servidor
	std::string	path = "html/" + request.getRoute();
	std::ifstream	fd(path.c_str());
	if (!fd)
	{
		sendError(404, client);
		return ;
	}
	fd.close();
	std::remove(path.c_str());

	std::string	msg = "HTTP/1.1 200 OK\n";
	msg += "Content-Length: 0\r\n\r\n";

	if (send(client.data.fd, msg.c_str(), msg.size(), 0) <= 0)
		sendError(500, client);
}


//SETTERS
void    Response::setErrors(void)
{
    _errors[200] = "200 OK";
	_errors[201] = "201 Created";
	_errors[204] = "204 No Content";
	_errors[300] = "300 Multiple Choices";
	_errors[301] = "301 Moved Permanently";
	_errors[302] = "302 Found";
	_errors[303] = "303 See Other";
	_errors[307] = "307 Temporary Redirect";
	_errors[400] = "400 Bad Request";
	_errors[404] = "404 Not Found";
	_errors[405] = "405 Method Not Allowed";
	_errors[408] = "408 Request Time Out";
	_errors[411] = "411 Length Required";
	_errors[413] = "413 Request Entity Too Large";
	_errors[414] = "414 Request-URI Too Long";

	_errors[418] = "418 I'm a teapot";

	_errors[500] = "500 Internal Server Error";
	_errors[502] = "502 Bad Gateway";
	_errors[505] = "505 HTTP Version Not Supported";
}

void    Response::setFileTypes(void)
{
    _fileTypes["png"] = "image/png";
    _fileTypes["jpg"] = "image/jpg";
    _fileTypes["gif"] = "image/gif";
    _fileTypes["html"] = "text/html";
    _fileTypes["css"] = "text/css";
    _fileTypes["pdf"] = "application/pdf";
    _fileTypes["json"] = "application/json";
    _fileTypes["mp4"] = "video/mp4";
    _fileTypes["default"] = "text/plain";
}


//GETTERS
std::string	Response::findType(std::string page)
{
	std::string	tmp = page;
	tmp.erase(0, page.find_last_of(".") + 1);

    if (_fileTypes[tmp].empty())
        return _fileTypes["default"];
    return _fileTypes[tmp];
}

std::string Response::getError(int n)
{
	if (_errors[n].empty())
		return "";
	return _errors[n];
}


//UTILS
bool		checkIndex(std::string path, std::string index)
{
	std::string indexFile = path + '/' + index;
	struct stat stat_index;
	if (stat(indexFile.c_str(), &stat_index) == 0 && S_ISREG(stat_index.st_mode))
		return true;
	return false;
}

std::string	ft_size_to_str(size_t n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}
