#include "../incs/WebServer.hpp"

void WebServer::validFileName(const std::string& fileName)
{
    if (fileName.size() < 5 || fileName.substr(fileName.size() - 5) != ".conf")
        throw MyException("Error: Config file Extension not found");

    const std::string fullPath = "config/" + fileName;

    std::ifstream file(fullPath.c_str());
    if (!file.good())
        throw MyException("Error: File not found in the config folder");
}

void WebServer::selectLine (const std::string &line)
{
    size_t start = 0;

    while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
        ++start;

    if (start >= line.size() || line[start] == '#')
        return;

    size_t commentPos = line.find('#', start);

    std::string trimmedLine = (commentPos != std::string::npos) ? line.substr(start, commentPos - start) : line.substr(start);

    int end = trimmedLine.size() - 1;
    while (end >= 0 && (trimmedLine[end] == ' ' || trimmedLine[end] == '\t' || trimmedLine[end] == '\r' || trimmedLine[end] == '\n'))
    {
       end--;
    }

    trimmedLine = trimmedLine.substr(0, end + 1);

    if (!trimmedLine.empty())
    {
        char lastChar = trimmedLine[trimmedLine.size() - 1];
        if (lastChar == '{' || lastChar == '}' || lastChar == ';')
            raw_file.push_back(trimmedLine);
        else
        {
            throw MyException("Error: Syntax error");
        }
    }
}


void WebServer::readConfFile(const std::string& fileName)
{
    const std::string fullPath = "config/" + fileName;

    std::ifstream file(fullPath.c_str());
    if (!file.is_open())
        throw MyException("Error: The File couldn't be openned");
    
    std::string line;
    while (std::getline(file, line))
    {
        WebServer::selectLine(line);
    }
    file.close();

}

std::string trimLeadingSpaces(const std::string& str) {
    // Encuentra el primer carácter que no sea espacio (' ') ni tabulación ('\t')
    std::size_t start = str.find_first_not_of(" \t");

    // Si encuentra un carácter que no sea espacio ni tabulación
    if (start != std::string::npos) {
        return str.substr(start);
    }

    // Si la cadena está vacía o solo tiene espacios/tabulaciones, retorna una cadena vacía
    return "";
}


void WebServer::manageLocationBracket(std::vector<std::string>::iterator &line, ServerConfig &sc)
{
    int brackets = 1;
    bool root = false;
    bool allow_methods = false;
    bool directory_listing = false;
    bool index = false;
    (void)sc;

    while (brackets != 0  && (line != raw_file.end()))
    {
        //std::cout << "      " << *line << std::endl;
        if (*line == "}")
            brackets--;
        else
        {
            if (line->find("root") == 0)
            {
                if (root == true)
                    throw MyException ("Error: duplicated root on location");
                root = true;
            }
            else if (line->find("allow_methods") == 0)
            {
                if (allow_methods == true)
                    throw MyException("Error: duplicated allow_methods on location");
                allow_methods = true;
            }
            else if (line->find("directory_listing") == 0)
            {
                if (directory_listing == true)
                    throw MyException("Error: duplicated directory_listing on location");
                directory_listing = true;
            }
            else if (line->find("index") == 0)
            {
                if (index == true)
                    throw MyException("Error: duplicated index on location");
                index = true;
            }
            else
                throw MyException("Error: Syntax error on location");
        }
        line++;
    }
    line--;
    line--;    //volvemos atras para evaluar el }
    if (root == false || allow_methods == false || directory_listing == false || index == false)
        throw MyException("Error: Missing info in location");
    // std::cout << "exiting line= " << *line << std::endl;
}

void WebServer::manageServerBracketVar(std::vector<std::string>::iterator &line, bool &listen, bool &server_name, bool &client_max, bool &error_pages, ServerConfig &sc)
{
    (void)sc;
    if (line->find("listen") == 0)
    {
        if (listen == true)
            throw MyException ("Error: duplicated listen on server");
        listen = true;
        //line->substr(std::string("listen ").length())
    }
    else if (line->find("server_name") == 0)
    {
        if (server_name == true)
            throw MyException ("Error: duplicated server_name on server");
        server_name = true;
        //line->substr(std::string("server_name ").length())
    }
    else if (line->find("client_max_body_size") == 0)
    {
        if (client_max == true)
            throw MyException ("Error: duplicated client_max_body_size on server");
        client_max = true;
        //line->substr(std::string("client_max_body_size ").length());
    }
    else if (line->find("error_page") == 0)
    {
        error_pages = true;
        //line->substr(std::string("error_page").length());
    }

}

void WebServer::manageServerBracket(std::vector<std::string>::iterator &line)
{
    int brackets = 1;
    const std::string prefix = "location /";

    //Create a serverConfig obj
    ServerConfig sc;
    while (brackets != 0  && (line != raw_file.end()))
    {
        
        //std::cout << "  " << *line <<std::endl;
            
        // Verificar si la longitud de la línea es suficiente
        if ((*line).size() >= prefix.size())
        {
            if ((*line).substr(0, prefix.size()) == prefix)
            {
                std::string newLine = line->substr(std::string(prefix).length());
                std::string locationName = std::string(newLine, 0, newLine.find_first_of(" "));

                //Locations location;
                std::cout << "location encontrado: " << locationName << std::endl;
                //manageLocationBracket(line, sc);
                brackets++;
            }
        }
        else if (*line == "}")
            brackets--;
        else
        {
            //manegar las variables sueltas
            //manageServerBracketVar(line, listenFlag, serverNameFlag, clientMaxFlag, errorPagesFlag, sc);           
        }
            
        line++;
    }

    // if (locationFlag == false || uploadFlag == false || cgiFlag == false || listenFlag == false || serverNameFlag == false || errorPagesFlag == false)
    //     throw MyException("Error: Missing info inside the server");
    serverConfigs.push_back(sc);
    line--;    //volvemos atras para evaluar el }
    line--;
    // std::cout << "exiting line= " << *line << std::endl;
}

void WebServer::parseFile()
{
    bool serverFlag = false;
    int brackets = 0;

    if (raw_file.empty())
        throw MyException("Errors: Server not found");

    for (std::vector<std::string>::iterator line = raw_file.begin(); line != raw_file.end(); line++)
    {
        std::cout << *line << std::endl;

        if (*line == "server {")
        {
            if (!serverFlag)
            {
                serverFlag = true;
                brackets++;
                manageServerBracket(++line);
            }
            else
                throw MyException("Error: Server nested");
        }
        else if (*line == "}")
        {
            //server bracket closed
            serverFlag = false;
            brackets--;
        }
        else
            throw MyException("Error: Syntax error (server)");
    }
    if (brackets !=0)
        throw MyException("Error: Check the brackets!");
}