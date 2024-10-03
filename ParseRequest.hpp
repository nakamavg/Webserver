/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anurtiag <anurtiag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:39:11 by anurtiag          #+#    #+#             */
/*   Updated: 2024/10/01 11:14:20 by anurtiag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

# include <iostream>
# include <string>
# include <fstream>

# include "MyException.hpp"

# define ENDLINE "\r\n"

class ParseRequest
{
    private:
        std::string _html;
        std::string _method;
        std::string _route;
        std::string _version;
        size_t      _lenght;
    public:
        ParseRequest(std::string _raw_request);
        ParseRequest(const ParseRequest& source);
        ParseRequest& operator=(const ParseRequest& source);
        ~ParseRequest();
        void RequestMethod();
        void ParseHead();
        void ParseBody();
};

std::string ft_read(std::string filename);

#endif