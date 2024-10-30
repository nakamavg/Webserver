#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "WebServer.hpp"

struct Locations {
    std::string id;                             
    std::string path;                           
    std::vector<std::string> allowed_methods;   
    std::string redirect;                       
    bool autoindex;
    std::string index;                         
    std::string default_file;                   
    std::string upload_dir;                     
    bool upload_enable;                         
    std::string path_info;                      
    bool cgi_dir;                  

    Locations() 
        : id(""),
          path(""),
          allowed_methods(),
          redirect(""), 
          autoindex(false), 
          index(""),
          default_file(""), 
          upload_dir(""), 
          upload_enable(false), 
          path_info(""), 
          cgi_dir(false) {}
};
class WebServer;

class ServerConfig
{
    public:
        ServerConfig();
        ServerConfig(const ServerConfig &copy);
        ~ServerConfig();
        ServerConfig &operator=(const ServerConfig &src);

        ServerConfig manageServerBracket(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file);
        void manageServerBracketVar(std::vector<std::string>::iterator &line, ServerConfig &sc);

        Locations manageLocationBracket(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file);
        void manageLocationBracketVar(std::vector<std::string>::iterator line, Locations &location);

        void addLocation(std::vector<std::string>::iterator &line, std::vector<std::string> raw_file);
        void addErrors(std::string line);

        void printLocation(Locations location);
        void printServerConfig(ServerConfig sc);

        std::string getHost() const { return host; }
        void setHost(const std::string &newHost) { host = newHost; }

        int getPort() const { return port; }
        void setPort(int newPort) { port = newPort; }

        std::vector<std::string> getServerNames() const { return server_names; }
        void setServerNames(const std::vector<std::string> &newServerNames) { server_names = newServerNames; }

        size_t getClientMaxBodySize() const { return client_max_body_size; }
        void setClientMaxBodySize(size_t newSize) { client_max_body_size = newSize; }

        std::map<int, std::string> getErrorPages() const { return error_pages; }
        void setErrorPages(const std::map<int, std::string> &newErrorPages) { error_pages = newErrorPages; }

        std::map<std::string, Locations> getLocations() const { return locations; }
        void setLocations(const std::map<std::string, Locations> &newLocations) { locations = newLocations; }
    
        std::string getDefRoot() const { return def_root; }
        void setDefRoot(const std::string &newDef_root) { def_root = newDef_root; }

        std::string getDefIndex() const { return def_index; }
        void setDefIndex(const std::string &newDef_index) { def_index = newDef_index; }

    private:
        std::string host;                               
        size_t port;                                      
        std::vector<std::string> server_names;          
        size_t client_max_body_size;                   
        std::map<int, std::string> error_pages;         
        std::map<std::string, Locations> locations;    
        
        std::string def_root;
        std::string def_index;
};

#endif