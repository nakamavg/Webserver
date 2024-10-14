#ifndef _PARSEREQUEST_HPP_
# define _PARSEREQUEST_HPP_

# include <iostream>
# include <string>
# include <fstream>

# include "MyException.hpp"
# include "ServerUp.hpp"

# define ENDLINE "\r\n"

class WebServer;

class ParseRequest
{
    private:

        std::string     _html;
        std::string     _method;
        std::string     _route;
        std::string     _version;
        std::string     _cgi_body;
        std::string     _boundary;

        std::map<std::string, std::string>     _rHeader;
        std::string                            _rLine;
        std::string                            _rBody;
        size_t                                 _length;

        std::string     _request;

        ServerConfig*   _conf;//

        ParseRequest( void );

    public:

        //ParseRequest(std::string _raw_request, std::vector<ServerConfig> &list);

        ParseRequest( std::string _raw_request );
        ParseRequest( const ParseRequest& source );
        ParseRequest& operator=( const ParseRequest& source );
        ~ParseRequest( void );

        void    makeCgiBody( void );
        void    makeGet( std::stringstream & strs );
        void    makePost( std::stringstream & strs );

        int     checkProt( void );

    //GETTERS
		std::string		getRequest( void );
		std::string		getMethod( void );
		std::string		getRoute( void );
		std::string		getProtocol( void );
		size_t			getLength( void );

		std::string		getBoundary( void );
		std::string		getBody( void );
		std::map<std::string, std::string>		getHeader( void );

        /*void ParseLine();
        void ParseHead();
        void ParseBody();
        void HasChunked();*/

};

std::string ft_read(std::string filename);

bool    checkRequest( std::string request );
bool	checkMethod(std::string method, std::vector<std::string> allowed_methods);

#endif