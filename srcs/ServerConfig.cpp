
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
        throw MyException("Error: Config file Extension not found");

    const std::string fullPath = "config/" + fileName;

    std::ifstream file(fullPath.c_str());
    if (!file.good())
        throw MyException("Error: File not found in the config folder");
}

void ServerConfig::selectLine (const std::string &line)
{
    size_t start = 0;

    while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
        ++start;

    if (start >= line.size() || line[start] == '#')
        return;

    size_t commentPos = line.find('#', start);

    std::string trimmedLine = (commentPos != std::string::npos) ? line.substr(start, commentPos - start) : line.substr(start);

    int end = trimmedLine.size();
    while (end >= 0 && (trimmedLine[end] == ' ' || trimmedLine[end] == '\t' || trimmedLine[end] == '\r' || trimmedLine[end] == '\0'))
    {
       end--;
    }
    char lastChar = trimmedLine[end];
    if (!trimmedLine.empty())
    {
        if (lastChar == '{' || lastChar == '}' || lastChar == ';')
            raw_file.push_back(trimmedLine);
        else
        {
            throw MyException("Error: Syntax error");
        }
    }
}

void ServerConfig::readConfFile(const std::string& fileName)
{
    const std::string fullPath = "config/" + fileName;

    std::ifstream file(fullPath.c_str());
    if (!file.is_open())
        throw MyException("Error: The File couldn't be openned");
    
    std::string line;
    while (std::getline(file, line))
    {
        ServerConfig::selectLine(line);
    }
    file.close();

}

void ServerConfig::parseFile()
{
    std::string str;

    for (std::vector<std::string>::iterator it = raw_file.begin(); it != raw_file.end(); ++it)
    {
        str = *it;
        std::cout << str << std::endl;
    }
}