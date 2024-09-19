

#include "incs/ServerConfig.hpp"

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        // std::string fileName = "default.conf";
        try
        {
            validFileName(argv[1]);
            std::cout << "all gud" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

}