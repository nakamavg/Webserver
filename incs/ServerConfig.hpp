#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <string>
# include <stack>
# include <sstream>

# include "MyException.hpp"

struct Locations {
    std::string id;                             //Identificador del location
    std::string path;                           // Directorio raíz para esta ruta
    std::vector<std::string> allowed_methods;   // Lista de métodos HTTP permitidos
    std::string redirect;                       // URL de redirección
    bool autoindex;
    std::string index;                            // Listado de directorios activado/desactivado
    std::string default_file;                   // Archivo por defecto para responder si se solicita un directorio
    std::string upload_dir;                     // Directorio para subir archivos
    bool upload_enable;                         // Habilitar la carga de archivos
    std::string path_info;                      // Ruta de información para el CGI
    std::string cgi_extension;                  // Extensión de archivo que activa el CGI

    // Constructor por defecto
    Locations() 
        : id(""),
          path(""), 
          allowed_methods(), // Inicializa el vector vacío
          redirect(""), 
          autoindex(false), 
          index(""),
          default_file(""), 
          upload_dir(""), 
          upload_enable(false), 
          path_info(""), 
          cgi_extension("") {} // Puedes usar el inicializador para strings
};

class ServerConfig
{
    public:
        ServerConfig();
        ServerConfig(const ServerConfig &copy);
        ~ServerConfig();
        ServerConfig &operator=(const ServerConfig &src);

        ServerConfig manageServerBracket(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file);
        
        Locations manageLocationBracket(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file);

        void manageServerBracketVar(std::vector<std::string>::iterator &line, ServerConfig &sc);

        void addLocation(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file);

        void printLocation(Locations location);
        void printServerConfig(ServerConfig sc);
        std::vector<std::string> parseMethods(const std::string& input);

        // Getter y Setter para host
        std::string getHost() const { return host; }
        void setHost(const std::string &newHost) { host = newHost; }

        // Getter y Setter para port
        int getPort() const { return port; }
        void setPort(int newPort) { port = newPort; }

        // Getter y Setter para server_names
        std::vector<std::string> getServerNames() const { return server_names; }
        void setServerNames(const std::vector<std::string> &newServerNames) { server_names = newServerNames; }

        // Getter y Setter para client_max_body_size
        size_t getClientMaxBodySize() const { return client_max_body_size; }
        void setClientMaxBodySize(size_t newSize) { client_max_body_size = newSize; }

        // Getter y Setter para error_pages
        std::map<int, std::string> getErrorPages() const { return error_pages; }
        void setErrorPages(const std::map<int, std::string> &newErrorPages) { error_pages = newErrorPages; }

        // Getter y Setter para locations
        std::map<std::string, Locations> getLocations() const { return locations; }
        void setLocations(const std::map<std::string, Locations> &newLocations) { locations = newLocations; }
    
    private:
        // Configuración general del servidor
        std::string host;                               // Nombre del servidor para el manejo de virtual hosts
        int port;                                       // Define el puerto en el que el servidor escuchará conexiones
        std::vector<std::string> server_names;          // Nombres de dominio aceptados
        size_t client_max_body_size;                    // Tamaño máximo del cuerpo de la solicitud
        std::map<int, std::string> error_pages;         // Páginas de error por defecto (código -> archivo)
        std::map<std::string, Locations> locations;     // Locations 
        
        //Locations locations;
};

#endif