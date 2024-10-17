#include "../incs/WebServer.hpp"
#include "../incs/ServerUp.hpp"

int main(int argc, char **argv)
{
    std::string fileName;

    if (argc == 1)
        fileName = "default.conf";
    else if (argc == 2)
        fileName = argv[1];
    else
        return (std::cout << "Usage: ./testeo [filename].conf" << std::endl, 2);

    try
        {
            WebServer ws;
            
            ws.validFileName(fileName);
            ws.readConfFile(fileName);
            ws.parseFile();
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