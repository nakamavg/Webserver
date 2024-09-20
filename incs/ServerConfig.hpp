#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include "MyException.hpp"

class ServerConfig
{
    public:
        ServerConfig() : port(80), client_max_body_size(0) {};
        ServerConfig(const ServerConfig &copy);
        virtual ~ServerConfig();
        ServerConfig &operator=(const ServerConfig &src);

        void selectLine (const std::string &line);
        void readConfFile(const std::string& fileName);
        void validFileName(const std::string& fileName);
        void parseFile ();

    private:
        // Configuración general del servidor
        std::string host;                               // Nombre del servidor para el manejo de virtual hosts
        int port;                                       // Define el puerto en el que el servidor escuchará conexiones
        std::vector<std::string> server_names;          // Nombres de dominio aceptados
        size_t client_max_body_size;                    // Tamaño máximo del cuerpo de la solicitud
        std::map<int, std::string> error_pages;         // Páginas de error por defecto (código -> archivo)
        std::vector<std::string> raw_file;

        // Configuración de rutas
        struct RouteConfig {
            std::string path;                           // Directorio raíz para esta rutan
            std::vector<std::string> allowed_methods;   // Lista de métodos HTTP permitidos
            //std::string redirect;                       // URL de redirección
            bool directory_listing;                     // Listado de directorios activado/desactivado
            std::string default_file;                   // Archivo por defecto para responder si se solicita un directorio

            RouteConfig() : directory_listing(false) {} // Constructor por defecto
        };

        //Configuracion de subida
        struct UploadConfig {
            std::string upload_dir;                     // Directorio para subir archivos
            bool upload_enable;                         // Habilitar la carga de archivos

            UploadConfig(){};
        };

        // Configuración de CGI
        struct CGIConfig {
            std::string path_info;                      // Ruta de información para el CGI
            std::string cgi_extension;                  // Extensión de archivo que activa el CGI

           CGIConfig(){};
        };

        CGIConfig cgi_config;                           // Configuración CGI
        RouteConfig route_config;                       // Configuración rutas
        UploadConfig upload_config;                     // Configuración subida

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

        // Getter y Setter para raw_file
        std::vector<std::string> getRawFile() const { return raw_file; }
        void setRawFile(const std::vector<std::string> &newRawFile) { raw_file = newRawFile; }
};

#endif