#include "serverlist.hpp"

serverlist::serverlist(const std::string &ip, size_t port) : ip(ip), port(port)
{}
const std::string & serverlist::get_ip()
{
	return this->ip;
}
size_t& serverlist::get_port()
{
	return this->port;
}


