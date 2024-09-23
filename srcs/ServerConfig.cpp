
#include "../incs/ServerConfig.hpp"

ServerConfig::ServerConfig(const ServerConfig &copy)
{
    host = copy.host;
    port = copy.port;
    server_names = copy.server_names;
    client_max_body_size = copy.client_max_body_size;
    error_pages = copy.error_pages;

    cgi_config = copy.cgi_config;
    route_config = copy.route_config;
    upload_config = copy.upload_config;
}

ServerConfig::~ServerConfig()
{
}

ServerConfig &ServerConfig::operator=(const ServerConfig &src)
{
    if (this == &src)
        return *this;

    host = src.host;
    port = src.port;
    server_names = src.server_names;
    client_max_body_size = src.client_max_body_size;
    error_pages = src.error_pages;

    cgi_config = src.cgi_config;
    route_config = src.route_config;
    upload_config = src.upload_config;

    return *this;
}
