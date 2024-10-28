#include "../incs/WebServer.hpp"
#include "../incs/ServerUp.hpp"


int main(int argc, char **argv)
{
    std::string fileName = "default.conf";
    int attempt = 0;
    if (argc == 2)
    {
        fileName = argv[1];
    } else if (argc > 2)
    {
        return (std::cout << "Usage: ./testeo [filename].conf <--(optional)" << std::endl, 2);
    }


    
    while (attempt < 2) {
        try {
            std::cout << std::endl;
            std::cout << "Reading file: " << YELLOW << fileName << NC << "...." << std::endl;

            WebServer ws;
            ws.validFileName(fileName);
            ws.readConfFile(fileName);
            ws.parseFile();
            ServerUp server(ws.getServerConfigs());
            server.start();
            return 0;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            attempt++;
            // we switch to default if the other filename is provided
            if (attempt == 1 && fileName != "default.conf")
            {
                fileName = "default.conf";
            }
            else
                break;
        }
    }

    std::cerr << "Both attempts failed. Exiting." << std::endl;
    return 1;
}