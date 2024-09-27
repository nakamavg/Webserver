
#include "../incs/ServerConfig.hpp"

ServerConfig::ServerConfig(){}// : host(NULL), port(0), client_max_body_size(0){}

ServerConfig::ServerConfig(const ServerConfig &copy) {
    this->host = copy.host;
    this->port = copy.port;
    this->server_names = copy.server_names;
    this->client_max_body_size = copy.client_max_body_size;
    this->error_pages = copy.error_pages;
    this->locations = copy.locations;
}

ServerConfig::~ServerConfig(){}

ServerConfig &ServerConfig::operator=(const ServerConfig &src) {
    if (this != &src) {
        this->host = src.host;
        this->port = src.port;
        this->server_names = src.server_names;
        this->client_max_body_size = src.client_max_body_size;
        this->error_pages = src.error_pages;
        this->locations = src.locations;
    }
    return *this;
}
