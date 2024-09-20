

#include "incs/ServerConfig.hpp"

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        // std::string fileName = "default.conf";
        try
        {
            ServerConfig serverConfig;

            serverConfig.validFileName(argv[1]);
            serverConfig.readConfFile(argv[1]);
            serverConfig.parseFile();
            //std::cout << "all gud" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
        std::cout << "Usage: ./testeo [filename].conf" << std::endl;
}