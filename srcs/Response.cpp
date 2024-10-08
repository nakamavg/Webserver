#include "../incs/Response.hpp"

Response::Response (std::string &html)
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
}