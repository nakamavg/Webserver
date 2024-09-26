#pragma once
#include "../Incl/Libs.hpp"

class serverlist
{
public:
 	serverlist(const std::string &ip, size_t port);
	const std::string&	get_ip();
	size_t & get_port();
	private:
		std::string ip;
		size_t port;
};