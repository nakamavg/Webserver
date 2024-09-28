

#include "incs/WebServer.hpp"
int main(int argc, char **argv)
{
    if (argc == 2)
    {
        // std::string fileName = "default.conf";
        try
        {
            WebServer ws;

            ws.validFileName(argv[1]);
            ws.readConfFile(argv[1]);
            ws.parseFile();
            std::cout << "Number of servers found: " << ws.getServerConfigs().size()  << std::endl;

            //std::cout << ws.getServerConfigs()[0].getLocations().size() << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
        std::cout << "Usage: ./testeo [filename].conf" << std::endl;
}