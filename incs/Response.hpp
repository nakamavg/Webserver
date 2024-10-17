#pragma once

#ifndef _RESPONSE_HPP_
# define _RESPONSE_HPP_

# include "Libs.hpp"
# include "../incs/ParseRequest.hpp"
# include "../incs/ServerConfig.hpp"
# include <sys/stat.h>

/*class Response
{
  public:
	Response(std::string &html);
	~Response();
	std::string get_web();

  private:
	std::string web;
};*/


class ParseRequest;

class Response
{

	private:

		std::map<std::string, std::string>	_fileTypes;
		std::map<int, std::string>			_errors;
		std::string							_default_error;
		ServerConfig 						*_conf;

		Response( void );

	//SETTERS
		void	setErrors( void );
		void	setFileTypes( void );

	public:

		Response( ServerConfig * conf );
		Response( const Response & source );
		Response & operator=( const Response & source );
		~Response( void );

		void	sendPage( std::string page, epoll_event & client, std::string request, int error );
		void	sendError( int error, epoll_event & client );
		void	sendChuncked( std::string, epoll_event & client, int error );
		bool	writePost( std::string path, epoll_event & client, std::string str );


	//METHODS
		void	metodGet( epoll_event & client, ParseRequest & request);
		void	metodPost( epoll_event & client, ParseRequest & request);
		void	metodDelete( epoll_event & client, ParseRequest & request);

	//GETTERS
		std::string	findType( std::string page );
		std::string	getError( int n );

};

//UTILS
bool		checkIndex(std::string path, std::string index);
std::string	ft_size_to_str(size_t n);

#endif