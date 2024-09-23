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
        void parseFile ();

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