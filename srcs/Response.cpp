#include "../incs/Response.hpp"

Response::Response (std::string &html)
{
	this->web =
					"HTTP/1.1 200 OK\r\n"
					"Content-Type: text/html\r\n"
					"Content-Length: " + std::to_string(html.size()) + "\r\n"
					"\r\n" + html.c_str() ;
}
Response::~Response(){};
std::string Response::get_web()
{
	return this->web;
}