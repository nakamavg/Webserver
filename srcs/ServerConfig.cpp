
#include "../incs/ServerConfig.hpp"

ServerConfig::ServerConfig(const ServerConfig &copy)
{
    host = copy.host;
    port = copy.port;
    server_names = copy.server_names;
    client_max_body_size = copy.client_max_body_size;
    error_pages = copy.error_pages;
    raw_file = copy.raw_file;

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
    raw_file = src.raw_file;

    cgi_config = src.cgi_config;
    route_config = src.route_config;
    upload_config = src.upload_config;

    return *this;
}


void ServerConfig::validFileName(const std::string& fileName)
{
    if (fileName.size() < 5 || fileName.substr(fileName.size() - 5) != ".conf")
        throw FileExtensionException();

    const std::string fullPath = "config/" + fileName;

    std::ifstream file(fullPath.c_str());
    if (!file.good())
        throw FileLocationException();
}

void ServerConfig::parseLine (const std::string &line)
{
    size_t start = 0;
    while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
        ++start;

    if (start >= line.size() || line[start] == '#')
        return;

    std::string trimmedLine = line.substr(start);
    if (trimmedLine.find_first_not_of(" \t") != std::string::npos)
    {
        raw_file.push_back(trimmedLine);
        std::cout << "pushing line: " << trimmedLine << std::endl;
    }
}

void ServerConfig::readConfFile(const std::string& fileName)
{
    const std::string fullPath = "config/" + fileName;

    std::ifstream file(fullPath.c_str());
    if (!file.is_open())
        throw FileOpeningException();
    
    std::string line;
    while (std::getline(file, line))
    {
        ServerConfig::parseLine(line);
    }
    file.close();

    // std::cout << "READING FROM RAW_FILE" << std::endl;
    // for (std::size_t i = 0; i < raw_file.size(); ++i) {
    //     std::cout << raw_file[i] << " ";
    // }
    // std::cout << std::endl;

}