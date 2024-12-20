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
        std::string     _url;
        std::string     _version;
        std::string     _cgi_body;
        std::string     _boundary;
        std::string     _fileName;

        std::map<std::string, std::string>     _rHeader;
        std::string                            _rLine;
        std::string                            _rBody;
        std::string                            _full_body;
        size_t                                 _length;

        std::string     _request;

        ParseRequest( void );

    public:

        ParseRequest( std::string _raw_request );
        ParseRequest( const ParseRequest& source );
        ParseRequest & operator=( const ParseRequest & source );
        ~ParseRequest( void );

        void    makeCgiBody( void );
        void    makeGet( std::stringstream & strs );
        void    makePost( std::stringstream & strs );

        int     checkProt( void );

    //GETTERS
		std::string		getRequest( void );
		std::string		getMethod( void );
		std::string		getRoute( void );
        std::string     getUrl( void );
		std::string		getProtocol( void );
		size_t			getLength( void );

		std::string		getBoundary( void );
        std::string     getFileName( void );
		std::string		getBody( void );
        std::string     getFullBody( void );
        std::string     getBodyCgi( void );

		std::map<std::string, std::string>		getHeader( void );

};

std::string ft_read(std::string filename);

bool    checkRequest( std::string request );
bool	checkMethod(std::string method, std::vector<std::string> allowed_methods);

#endif