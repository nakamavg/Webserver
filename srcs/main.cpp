#include "../incs/WebServer.hpp"
#include "../incs/ServerUp.hpp"

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
            std::cout << "\033[32m" << "OK" << "\033[0m" << std::endl;
            ServerUp a(ws.getServerConfigs());
            a.start();
            
            // std::cout << "Number of servers found: " << ws.getServerConfigs().size()  << std::endl;
            // std::cout << "Number of locations on the 1st server: " << ws.getServerConfigs()[0].getLocations().size() << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
        std::cout << "Usage: ./testeo [filename].conf" << std::endl;
}