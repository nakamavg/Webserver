/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anurtiag <anurtiag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:39:11 by anurtiag          #+#    #+#             */
/*   Updated: 2024/10/09 13:17:13 by anurtiag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

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
        std::string     _rLine;
        std::string     _rHeader;
        std::string     _rBody;
        size_t          _lenght;
        ServerConfig*   _conf;
    public:
        ParseRequest(std::string _raw_request, std::vector<ServerConfig> &list);
        ParseRequest(const ParseRequest& source);
        ParseRequest& operator=(const ParseRequest& source);
        ~ParseRequest();
        void ParseLine();
        void ParseHead();
        void ParseBody();
        void HasChunked();
};

std::string ft_read(std::string filename);

#endif