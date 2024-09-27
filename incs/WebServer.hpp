#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <string>
# include <stack>

# include "MyException.hpp"
# include "ServerConfig.hpp"

class WebServer
{
    public:
        void selectLine (const std::string &line);
        void readConfFile(const std::string& fileName);
        void validFileName(const std::string& fileName);
        void parseFile();
        void manageServerBracket(std::vector<std::string>::iterator &line);
        void manageLocationBracket(std::vector<std::string>::iterator &line, ServerConfig &sc);
        //void manageUploadsBracket(std::vector<std::string>::iterator &line, ServerConfig &sc);
        //void manageCgiBinBracket(std::vector<std::string>::iterator &line, ServerConfig &sc);
        void manageServerBracketVar(std::vector<std::string>::iterator &line, bool &listen, bool &server_name, bool &client_max, bool &error_pages, ServerConfig &sc);
        //void manageVarLine(int id, std::string value, ServerConfig &sc);

        const std::vector<ServerConfig>& getServerConfigs() const
        {
            return serverConfigs;
        }

        const std::vector<std::string>& getRawFile() const
        {
            return raw_file;
        }

        void setServerConfigs(const std::vector<ServerConfig>& configs)
        {
            serverConfigs = configs;
        }

        void setRawFile(const std::vector<std::string>& rawFileContent)
        {
            raw_file = rawFileContent;
        }

    private:
        std::vector<ServerConfig> serverConfigs;
        std::vector<std::string> raw_file;
};

#endif