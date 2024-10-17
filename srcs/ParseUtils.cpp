#include "../incs/ParseUtils.hpp"

int ft_stoi(std::string str)
{
    int number;

    std::stringstream ss(str);

    ss >> number;

    if (ss.fail())
    {
        throw MyException ("Error: Couldn't format string into int");
    }

    return number;
}

std::string trimLeadingSpaces(const std::string& str)
{
    // Encuentra el primer carácter que no sea espacio (' ') ni tabulación ('\t')
    std::size_t start = str.find_first_not_of(" \t");

    // Si encuentra un carácter que no sea espacio ni tabulación
    if (start != std::string::npos) {
        return str.substr(start);
    }

    // Si la cadena está vacía o solo tiene espacios/tabulaciones, retorna una cadena vacía
    return "";
}

std::vector<std::string> parseMethods(const std::string& input)
{
    std::vector<std::string> allowedMethods;
    std::vector<std::string> result;
    std::string word;

    allowedMethods.push_back("GET");
    allowedMethods.push_back("POST");
    allowedMethods.push_back("DELETE");

    std::istringstream stream(input);

    while (stream >> word)
    {
        if (!word.empty() && word[word.length() - 1] == ';')
        {
            word = word.substr(0, word.length() - 1);
        }

        bool isAllowed = false;
        for (size_t i = 0; i < allowedMethods.size(); ++i)
        {
            if (word == allowedMethods[i])
            {
                isAllowed = true;
                break;
            }
        }
        
        if (isAllowed == false)
            throw MyException("Error: Not allowed method found");

        for (size_t i = 0; i < allowedMethods.size(); ++i)
        {
            if (word == allowedMethods[i])
            {
                bool exists = false;
                for (size_t j = 0; j < result.size(); ++j)
                {
                    if (result[j] == word)
                    {
                        exists = true;
                        throw MyException("Error: Duplicated method");
                    }
                }
                if (!exists)
                {
                    result.push_back(word);
                }
                break;
            }
        }
    }

    return result;
}

void parseIpAddress(std::string valueLine, ServerConfig &sc)
{
    size_t colonPos = valueLine.find(':');
    if (colonPos != std::string::npos)
    {
        sc.setHost(valueLine.substr(0, colonPos));
        sc.setPort(std::atoi(valueLine.substr(colonPos + 1).c_str()));
    }
    else
        throw MyException("Error: syntax error near the listen(separator ':' not found)");
}

void parseCmbs(std::string str, ServerConfig &sc)
{
    if (str.size() < 2) {
        throw MyException("Error: invalid cmbs");
    }

    char lastChar = str[str.size() - 2];
    size_t cmbs = ft_stoi(str);
    switch (lastChar)
    {
    case 'K':
    /* kilobytes */
        cmbs = cmbs * 1024;
        break;
    case 'M':
    /* megabytes */
        cmbs = cmbs * 1024 * 1024;
        break;
    case 'G':
    /* gigabytes */
        cmbs = cmbs * 1024 * 1024 * 1024;
        break;
    
    default:
    /* bytes */
        break;
    }
    sc.setClientMaxBodySize(cmbs);
}

void checkPorts(const std::vector<ServerConfig> &serverConfigs)
{
    std::vector<int> uniquePorts;  // Vector para almacenar puertos únicos

    for (std::vector<ServerConfig>::const_iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it)
    {
        int port = (*it).getPort();  // Obtener el puerto actual

        // Verificar si el puerto ya está en el vector uniquePorts
        for (std::vector<int>::const_iterator portIt = uniquePorts.begin(); portIt != uniquePorts.end(); ++portIt)
        {
            if (*portIt == port)
            {
                throw MyException("Error: Duplicated Ports founds");
            }
        }

        // Si no se encontró duplicado, añadimos el puerto al vector uniquePorts
        uniquePorts.push_back(port);
    }
}
