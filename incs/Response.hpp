#pragma once

#ifndef _RESPONSE_HPP_
# define _RESPONSE_HPP_

# include "Libs.hpp"
# include "../incs/ParseRequest.hpp"
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

	//SETTERS
		void	setErrors( void );
		void	setFileTypes( void );

	public:

		Response( void );
		~Response( void );

		void	sendPage( std::string page, int socket, std::string request, int error );
		void	sendError( int error, int socket );
		void	sendChuncked( std::string, int socket, int error );

	//METHODS
		void	metodGet(int socket, ParseRequest & request);
		void	metodPost(int socket, ParseRequest & request);
		void	metodDelete(int socket, ParseRequest & request);

	//GETTERS
		std::string	findType( std::string page );
		std::string	getError( int n );

};

//UTILS
bool		checkIndex(std::string path, std::string index);
std::string	ft_size_to_str(size_t n);

#endif