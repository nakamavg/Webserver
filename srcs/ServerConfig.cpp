
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
    this->def_index = copy.def_index;
    this->def_root = copy.def_root;
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
        this->def_index = src.def_index;
        this->def_root = src.def_root;
    }
    return *this;
}


void ServerConfig::addLocation(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file)
{
    Locations loc = manageLocationBracket(line, raw_file);
    // Aquí usas `loc.id` como clave para el mapa
    //std::cout << "Adding location: " << loc.id << std::endl; // Debug output

    //std::cout << "location id: " << loc.id << loc.id.empty() << std::endl;

    if (loc.id.empty() == 1)
        loc.id = "Default";
    locations[loc.id] = loc; // Inserta el objeto en el mapa

}

void ServerConfig::addErrors(std::string line)
{
    int id;
    std::string html;

    std::istringstream stream(line);

    if (!(stream >> id))
        throw MyException("Error: Couln't read the id");

    stream >> html;

    if (html.empty() || html[0] != '/')
        throw MyException("Error: Html not valid");

    error_pages[id] = html;
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
    std::string cleanLine = *line;
    std::string valueLine;

    cleanLine.erase(cleanLine.size() - 1);

    if (cleanLine.compare(0, 5, "root ") == 0)
    {
        if (location.path != "")
            throw MyException("Error: duplicated root on location");

        valueLine = cleanLine.substr(std::string("root ").length());
        location.path = valueLine;
    }
    else if (cleanLine.compare(0, 14, "allow_methods ") == 0)
    {
        if (!location.allowed_methods.empty())
            throw MyException("Error: duplicated allow_methods on location");

        valueLine = cleanLine.substr(std::string("allow_methods ").length());
        location.allowed_methods = parseMethods(valueLine);
    }
    else if (cleanLine.compare(0, 18, "directory_listing ") == 0)
    {
        if (location.autoindex)
            throw MyException("Error: duplicated directory_listing on location");

        valueLine = cleanLine.substr(std::string("directory_listing ").length());
        location.autoindex = (valueLine == "on");
    }
    else if (cleanLine.compare(0, 6, "index ") == 0)
    {
        if (!location.index.empty())
            throw MyException("Error: duplicated index on location");

        valueLine = cleanLine.substr(std::string("index ").length());
        location.index = valueLine;
    }
    else if (cleanLine.compare(0, 13, "default_file ") == 0)
    {
        if (!location.default_file.empty())
            throw MyException("Error: duplicated default_file on location");
        
        valueLine = cleanLine.substr(std::string("default_file ").length());
        location.default_file = valueLine;
    }
    else if (cleanLine.compare(0, 11, "upload_dir ") == 0)
    {
        if (!location.upload_dir.empty())
            throw MyException("Error: duplicated upload_dir on location");

        valueLine = cleanLine.substr(std::string("upload_dir ").length());
        location.upload_dir = valueLine;
    }
    else if (cleanLine.compare(0, 14, "upload_enable ") == 0)
    {
        if (location.upload_enable)
            throw MyException("Error: duplicated upload_enable on location");

        std::cout << "patatatatatata" << std::endl;
        valueLine = cleanLine.substr(std::string("upload_enable ").length());
        location.upload_enable = (valueLine == "on");
    }
    else if (cleanLine.compare(0, 10, "path_info ") == 0)
    {
        if (!location.path_info.empty())
            throw MyException("Error: duplicated path_info on location");

        valueLine = cleanLine.substr(std::string("path_info ").length());
        location.path_info = valueLine;
    }
    else if (cleanLine.compare(0, 8, "cgi_dir ") == 0)
    {
        if (location.cgi_dir)
            throw MyException("Error: duplicated cgi_dir on location");

        valueLine = cleanLine.substr(std::string("cgi_dir ").length());
        location.cgi_dir = (valueLine == "on");
    }
    else if (cleanLine.compare(0, 7, "return ") == 0)
    {
        if (!location.redirect.empty())
            throw MyException ("Error: duplicated return on location");
        
        valueLine = cleanLine.substr(std::string("return ").length());
        location.redirect = valueLine;
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
    //std::cout << "port: " << sc.port << " host: " << sc.host << " index: " << sc.def_index << " root: " << sc.def_root << std::endl;
    if (sc.port == 0 || sc.host.empty() || sc.def_index.empty() || sc.def_root.empty())
        throw MyException("Error: Missing info to be able to open the server");
    if (sc.client_max_body_size == 0)
        sc.setClientMaxBodySize(120);
    sc.setLocations(getLocations());
    return sc;
}

void ServerConfig::manageServerBracketVar(std::vector<std::string>::iterator &line, ServerConfig &sc)
{
    std::string cleanLine = *line;
    std::string valueLine;

    cleanLine.erase(cleanLine.size() - 1); // Elimina el último carácter, que podría ser un ';' o un espacio

    if (cleanLine.compare(0, 7, "listen ") == 0)
    {
        if ((sc.port != 0) || sc.host != "")
            throw MyException("Error: duplicated listen on server");

        valueLine = cleanLine.substr(std::string("listen ").length());
        parseIpAddress(valueLine, sc);
    }
    else if (cleanLine.compare(0, 12, "server_name ") == 0)
    {
        if (!(sc.server_names).empty())
            throw MyException("Error: duplicated server_name on server");

        valueLine = cleanLine.substr(std::string("server_name ").length());
        sc.server_names.push_back(valueLine);
    }
    else if (cleanLine.compare(0, 21, "client_max_body_size ") == 0)
    {
        if (sc.client_max_body_size != 0)
            throw MyException("Error: duplicated client_max_body_size on server");

        valueLine = cleanLine.substr(std::string("client_max_body_size ").length());
        parseCmbs(valueLine, sc);
    }
    else if (cleanLine.compare(0, 11, "error_page ") == 0)
    {
        valueLine = cleanLine.substr(std::string("error_page ").length());
        addErrors(valueLine);
        sc.setErrorPages(error_pages);
    }
    else if (cleanLine.compare(0, 5, "root ") == 0)
    {
        if (!(sc.def_root).empty())
            throw MyException("Error: duplicated root on server");

        valueLine = cleanLine.substr(std::string("root ").length());
        sc.def_root = valueLine;
    }
    else if (cleanLine.compare(0, 6, "index ") == 0)
    {
        if (!(sc.def_index).empty())
            throw MyException("Error: duplicated index on server");

        valueLine = cleanLine.substr(std::string("index ").length());
        sc.def_index = valueLine;
    }
    else
    {
        throw MyException("Error: Syntax error on server");
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

        std::cout << "  CGI Dir: " << (location.cgi_dir ? "Enabled" : "Disabled") << std::endl;
    std::cout << std::endl;
}

void ServerConfig::printServerConfig(ServerConfig sc)
{
    std::cout << std::endl;
    std::cout << "Host: " << sc.host << std::endl;
    std::cout << "Port: " << sc.port << std::endl;

    if(!sc.server_names.empty())
    {
        std::cout << "Server Names: ";
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
        std::cout << "Error pages: ";

        std::map<int, std::string>::const_iterator it = sc.error_pages.begin();

        if (it != sc.error_pages.end())
        {
            std::cout << it->first << " " << it->second;
            ++it;

            for (; it != sc.error_pages.end(); ++it)
            {
                std::cout << ", " << it->first << " " << it->second;
            }
        }

        std::cout << std::endl;
    }

    if (!sc.def_index.empty())
        std::cout << "DEF INDEX: " << sc.def_index << std::endl;

    if (!sc.def_root.empty())
        std::cout << "DEF ROOT: " << sc.def_root << std::endl;
    
    std::cout << std::endl;
    for (std::map<std::string, Locations>::iterator it = locations.begin(); it != locations.end(); ++it)
    {
        printLocation(it->second);
    }
}