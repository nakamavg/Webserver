/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyException.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anurtiag <anurtiag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 10:17:45 by anurtiag          #+#    #+#             */
/*   Updated: 2024/09/02 13:12:53 by anurtiag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYEXCEPTION_HPP
# define MYEXCEPTION_HPP

# include <exception>

class MyException : public std::exception
{
    private:
        const char* _message;
    public:
        MyException(const char* message) : _message(message) {}
        const char* what() const throw()
        {
            return(_message);
        }
};



#endif