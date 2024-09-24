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

void WebServer::manageLocationBracket(std::vector<std::string>::iterator &line)
{
    int brackets = 1;
    bool root = false;
    bool allow_methods = false;
    bool directory_listing = false;
    bool index = false;

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

void WebServer::manageUploadsBracket(std::vector<std::string>::iterator &line)
{
    int brackets = 1;
    bool root = false;
    bool upload_enabled = false;

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
                    throw MyException ("Error: duplicated root on location /uploads");
                root = true;
            }
            else if (line->find("upload_enabled") == 0)
            {
                if (upload_enabled == true)
                    throw MyException("Error: duplicated upload_enabled on location /uploads");
                upload_enabled = true;
            }
            else
                throw MyException("Error: Syntax error on location /uploads");
        }
        line++;
    }
    line--;
    line--;    //volvemos atras para evaluar el }
    if (root == false || upload_enabled == false)
        throw MyException("Error: Missing info in location /uploads");
    // std::cout << "exiting line= " << *line << std::endl;
}

void WebServer::manageCgiBinBracket(std::vector<std::string>::iterator &line)
{
    int brackets = 1;
    bool root = false;
    bool cgi_extension = false;

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
                    throw MyException ("Error: duplicated root on location /cgi-bin");
                root = true;
            }
            else if (line->find("cgi_extension") == 0)
            {
                if (cgi_extension == true)
                    throw MyException("Error: duplicated upload_enabled on location /cgi-bin");
                cgi_extension = true;
            }
            else
                throw MyException("Error: Syntax error on location /cgi-bin");
        }
        line++;
    }
    line--;
    line--;    //volvemos atras para evaluar el }
    if (root == false || cgi_extension == false)
        throw MyException("Error: Missing info in location /cgi-bin");
    // std::cout << "exiting line= " << *line << std::endl;
}

void WebServer::manageServerBracketVar(std::vector<std::string>::iterator &line, bool &listen, bool &server_name, bool &client_max, bool &error_pages)
{
    if (line->find("listen") == 0)
    {
        if (listen == true)
            throw MyException ("Error: duplicated listen on server");
        listen = true;
    }
    else if (line->find("server_name") == 0)
    {
        if (server_name == true)
            throw MyException ("Error: duplicated server_name on server");
        server_name = true;
    }
    else if (line->find("client_max_body_size") == 0)
    {
        if (client_max == true)
            throw MyException ("Error: duplicated client_max_body_size on server");
        client_max = true;
    }
    else if (line->find("error_page") == 0)
    {
        error_pages = true;
    }
}
void WebServer::manageServerBracket(std::vector<std::string>::iterator &line)
{
    int brackets = 1;
    bool locationFlag = false;
    bool uploadFlag = false;
    bool cgiFlag = false;

    bool listenFlag = false;
    bool serverNameFlag = false;
    bool clientMaxFlag = false;
    bool errorPagesFlag = false;

    //Create a serverConfig obj
    ServerConfig sc;
    while (brackets != 0  && (line != raw_file.end()))
    {
        
        //std::cout << "  " << *line <<std::endl;
        
        if (*line == "location / {")
        {
            if (!locationFlag)
            {
                locationFlag = true;
                brackets++;
                manageLocationBracket(++line);
            }
            else
                throw MyException("Error: Syntax error (location) duplicated");
        }
        else if (*line == "location /uploads {")
        {
            if (!uploadFlag)
            {
                uploadFlag = true;
                brackets++;
                manageUploadsBracket(++line);
            }
            else
                throw MyException("Error: Syntax error (uploads) duplicated ");
        }
        else if (*line == "location /cgi-bin {")
        {
            if (!cgiFlag)
            {
                cgiFlag = true;
                brackets++;
                manageCgiBinBracket(++line);
            }
            else
                throw MyException("Error: Syntax error (cgi-bin) duplicated");
        }
        else if (*line == "}")
            brackets--;
        else
            manageServerBracketVar(line, listenFlag, serverNameFlag, clientMaxFlag, errorPagesFlag);           
        line++;
    }

    if (locationFlag == false || uploadFlag == false || cgiFlag == false || listenFlag == false || serverNameFlag == false || errorPagesFlag == false)
        throw MyException("Error: Missing info inside the server");
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
        //std::cout << *line << std::endl;

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