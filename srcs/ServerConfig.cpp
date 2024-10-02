
#include "../incs/ServerConfig.hpp"

ServerConfig::ServerConfig()
{
    port = 0;
    client_max_body_size = 0;
}// : host(NULL), port(0), client_max_body_size(0){}

ServerConfig::ServerConfig(const ServerConfig &copy)
{
    this->host = copy.host;
    this->port = copy.port;
    this->server_names = copy.server_names;
    this->client_max_body_size = copy.client_max_body_size;
    this->error_pages = copy.error_pages;
    this->locations = copy.locations;
}

ServerConfig::~ServerConfig()
{}

ServerConfig &ServerConfig::operator=(const ServerConfig &src)
{
    if (this != &src)
    {
        this->host = src.host;
        this->port = src.port;
        this->server_names = src.server_names;
        this->client_max_body_size = src.client_max_body_size;
        this->error_pages = src.error_pages;
        this->locations = src.locations;
    }
    return *this;
}




void ServerConfig::addLocation(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file)
{
    Locations loc = manageLocationBracket(line, raw_file);
    // Aquí usas `loc.path` como clave para el mapa
    //std::cout << "Adding location: " << loc.id << std::endl; // Debug output

    if (locations.size() == 0)
        loc.id = "Default";
    locations[loc.id] = loc; // Inserta el objeto en el mapa

}

Locations ServerConfig::manageLocationBracket(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file)
{
    int brackets = 1;
    Locations location;  // Crear una instancia de Locations

    std::string newLine = line->substr(std::string("location /").length());
    location.id = std::string(newLine, 0, newLine.find_first_of(" "));
    line++;
    while (brackets != 0 && (line != raw_file.end()))
    {
        //std::cout << "      " << *line << std::endl;
        if (*line == "}")
            brackets--;
        else
            manageLocationBracketVar(line, location);
        line++;
    }
    line--; // Retroceder para evaluar el }
    line--;
    return location;
}

void ServerConfig::manageLocationBracketVar(std::vector<std::string>::iterator line, Locations &location)
{
    if (line->find("root") == 0)
            {
                if (location.path != "")
                    throw MyException("Error: duplicated root on location");
                location.path = line->substr(std::string("root ").length());
            }
            else if (line->find("allow_methods") == 0)
            {
                if (!location.allowed_methods.empty())
                    throw MyException("Error: duplicated allow_methods on location");
                std::string methods = line->substr(std::string("allow_methods ").length());
                location.allowed_methods = parseMethods(methods);
            }
            else if (line->find("directory_listing") == 0)
            {
                if (location.autoindex)
                    throw MyException("Error: duplicated directory_listing on location");
                std::string value = line->substr(std::string("directory_listing ").length());
                location.autoindex = (value == "on");
            }
            else if (line->find("index") == 0)
            {
                if (!location.index.empty())
                    throw MyException("Error: duplicated index on location");
                location.index = line->substr(std::string("index ").length());
            }
            else if (line->find("default_file") == 0)
            {
                if (!location.default_file.empty())
                    throw MyException("Error: duplicated default_file on location");
                location.default_file = line->substr(std::string("default_file ").length());
            }
            else if (line->find("upload_dir") == 0)
            {
                if (!location.upload_dir.empty())
                    throw MyException("Error: duplicated upload_dir on location");
                location.upload_dir = line->substr(std::string("upload_dir ").length());
            }
            else if (line->find("upload_enable") == 0)
            {
                if (location.upload_enable)
                    throw MyException("Error: duplicated upload_enable on location");
                std::string value = line->substr(std::string("upload_enable ").length());
                location.upload_enable = (value == "on");
            }
            else if (line->find("path_info") == 0)
            {
                if (!location.path_info.empty())
                    throw MyException("Error: duplicated path_info on location");
                location.path_info = line->substr(std::string("path_info ").length());
            }
            else if (line->find("cgi_extension") == 0)
            {
                if (!location.cgi_extension.empty())
                    throw MyException("Error: duplicated cgi_extension on location");
                location.cgi_extension = line->substr(std::string("cgi_extension ").length());
            }
            else if (line->find("return") == 0)
            {
                if (!location.redirect.empty())
                    throw MyException ("Error: duplicated return on location");
                location.redirect = line->substr(std::string("return ").length());
            }
            else
            {
                throw MyException("Error: Syntax error on location");
            }
}

ServerConfig ServerConfig::manageServerBracket(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file)
{
    int brackets = 1;
    const std::string prefix = "location /";

    //Create a serverConfig obj
    ServerConfig sc;
    while (brackets != 0  && (line != raw_file.end()))
    {

        // Verificar si la longitud de la línea es suficiente
        if (((*line).size() >= prefix.size()) && ((*line).substr(0, prefix.size()) == prefix))
        {
            addLocation(line, raw_file);
            brackets++;
        }
        else if (*line == "}")
        {
            brackets--;
        }
        else
        {
            //manegar las variables sueltas
            manageServerBracketVar(line, sc);
        }
            
        line++;
    }

    line--;    //volvemos atras para evaluar el }
    line--;
    //std::cout << "Number of Locations in this server: " << locations.size() << std::endl;
    if (sc.port == 0 && sc.host.empty())
        throw MyException("Error: Missing info to be able to open the server");
    return sc;
}

void ServerConfig::manageServerBracketVar(std::vector<std::string>::iterator &line, ServerConfig &sc)
{
    std::string valueLine;
    if (line->find("listen") == 0)
    {
        if ((sc.port != 0) || sc.host !="")
            throw MyException ("Error: duplicated listen on server");

        valueLine = line->substr(std::string("listen ").length());
        parseIpAddress(valueLine, sc);
    }
    else if (line->find("server_name") == 0)
    {
        if (!(sc.server_names).empty())
            throw MyException ("Error: duplicated server_name on server");

        valueLine = line->substr(std::string("server_name ").length());
        sc.server_names.push_back(valueLine);
    }
    else if (line->find("client_max_body_size") == 0)
    {
        if (sc.client_max_body_size != 0)
            throw MyException ("Error: duplicated client_max_body_size on server");

        valueLine = line->substr(std::string("client_max_body_size ").length());
        sc.client_max_body_size = ft_stoi(valueLine);
    }
    else if (line->find("error_page") == 0)
    {
        valueLine = line->substr(std::string("error_page").length());
    }
}




//----------------PRINTEOS----------------
void ServerConfig::printLocation(Locations location)
{
    std::cout << "Location ID: " << location.id << std::endl;

    if (!location.path.empty())
    {
        std::cout << "  Root: " << location.path << std::endl;
    }

    if (!location.allowed_methods.empty()) {
        std::cout << "  Allowed Methods: ";
        for (size_t i = 0; i < location.allowed_methods.size(); ++i) {
            std::cout << location.allowed_methods[i];
            if (i < location.allowed_methods.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    if (!location.redirect.empty()) {
        std::cout << "  Redirect: " << location.redirect << std::endl;
    }

    std::cout << "  Autoindex: " << (location.autoindex ? "Enabled" : "Disabled") << std::endl;

    if (!location.index.empty()) {
        std::cout << "  Index: " << location.index << std::endl;
    }

    if (!location.default_file.empty()) {
        std::cout << "  Default File: " << location.default_file << std::endl;
    }

    if (!location.upload_dir.empty()) {
        std::cout << "  Upload Directory: " << location.upload_dir << std::endl;
    }

    std::cout << "  Upload Enable: " << (location.upload_enable ? "Enabled" : "Disabled") << std::endl;

    if (!location.path_info.empty()) {
        std::cout << "  Path Info: " << location.path_info << std::endl;
    }

    if (!location.cgi_extension.empty()) {
        std::cout << "  CGI Extension: " << location.cgi_extension << std::endl;
    }
    std::cout << std::endl;
}

void ServerConfig::printServerConfig(ServerConfig sc)
{
    std::cout << "Host: " << sc.host << std::endl;
    std::cout << "Port: " << sc.port << std::endl;

    if(!sc.server_names.empty())
    {
        for (size_t i = 0; i < sc.server_names.size(); ++i)
        {
            std::cout << sc.server_names[i];
            if (i < sc.server_names.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    if (sc.client_max_body_size)
        std::cout << "CMBS: " << sc.client_max_body_size << std::endl;
    
    if(!sc.error_pages.empty())
    {
        for (size_t i = 0; i < sc.error_pages.size(); ++i)
        {
            std::cout << sc.error_pages[i];
            if (i < sc.error_pages.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    for (std::map<std::string, Locations>::iterator it = locations.begin(); it != locations.end(); ++it)
    {
        printLocation(it->second);
    }
}