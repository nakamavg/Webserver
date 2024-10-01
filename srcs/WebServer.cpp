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

                ServerConfig sc = sc.manageServerBracket(++line, raw_file);
                //devuelve una configuracion de servidor y la guardamos
                serverConfigs.push_back(sc);
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
    
    // for (std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it)
    // {
    //    (*it).printServerConfig((*it));
    // }
}