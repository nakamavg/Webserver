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
    std::string line;
    bool serverFlag = false;
    int brackets = 0;
    bool locationFlag = false;
    bool uploadFlag = false;
    bool cgiFlag = false;

    for (std::vector<std::string>::iterator it = raw_file.begin(); it != raw_file.end(); ++it)
    {
        line = *it;
        //std::cout << "-" << line << "-" << std::endl;

        if (line == "server {")
        {
            if (!serverFlag)
            {
                serverFlag = true;
                brackets++;
                //Create a serverConfig obj
                ServerConfig sc;
                serverConfigs.push_back(sc);
            }
            else
                throw MyException("Error: Syntax error (server)");
        }
        else if (line == "location / {")
        {
            if (!locationFlag)
            {
                locationFlag = true;
                brackets++;
            }
            else
                throw MyException("Error: Syntax error (location)");
        }
        else if (line == "location /uploads {")
        {
            if (!uploadFlag)
            {
                uploadFlag = true;
                brackets++;
            }
            else
                throw MyException("Error: Syntax error (uploads)");
        }
        else if (line == "location /cgi-bin {")
        {
            if (!cgiFlag)
            {
                cgiFlag = true;
                brackets++;
            }
            else
                throw MyException("Error: Syntax error (cgi-bin)");
        }
        else
        {
            for (size_t i = 0; i < line.size(); ++i)
            {
                if (line[i] == '{')
                    brackets++;
                else if (line[i] == '}')
                    brackets--;
            }
        }

        if (brackets == 0)
        {
            //server bracket closed
            serverFlag = false;
            locationFlag = false;
            uploadFlag = false;
            cgiFlag = false;
        }
    }
    if (brackets !=0)
        throw MyException("Error: Check the brackets!");
}