#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class FileExtensionException : public std::exception {
public:
    const char* what() const throw() {
        return "Error: Config file Extension not found";
    }
};

class FileLocationException : public std::exception {
public:
    const char* what() const throw() {
        return "Error: File not found in the config folder";
    }
};

class ServerConfig
{
    public:
        ServerConfig() : port(80), client_max_body_size(0) {};
        ServerConfig(const ServerConfig &copy);
        virtual ~ServerConfig();
        ServerConfig &operator=(const ServerConfig &src);

    private:
        // Configuración general del servidor
        std::string host;                               // Nombre del servidor para el manejo de virtual hosts
        int port;                                       // Define el puerto en el que el servidor escuchará conexiones
        std::vector<std::string> server_names;          // Nombres de dominio aceptados
        size_t client_max_body_size;                    // Tamaño máximo del cuerpo de la solicitud
        std::map<int, std::string> error_pages;         // Páginas de error por defecto (código -> archivo)

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

            UploadConfig();
        };

        // Configuración de CGI
        struct CGIConfig {
            std::string path_info;                      // Ruta de información para el CGI
            std::string cgi_extension;                  // Extensión de archivo que activa el CGI

            CGIConfig() {}
        };

        CGIConfig cgi_config;                           // Configuración CGI
        RouteConfig route_config;                       // Configuración rutas
        UploadConfig upload_config;                     // Configuración subida

};

void validFileName(const std::string& fileName);

#endif