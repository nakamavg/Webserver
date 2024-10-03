/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anurtiag <anurtiag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:38:10 by anurtiag          #+#    #+#             */
/*   Updated: 2024/10/01 09:28:19 by anurtiag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <string>
// #include <iostream>
// #include <vector>

#include "ParseRequest.hpp"

int main(int argc, char** argv)
{
    std:: string get = ft_read("get.txt");
    ParseRequest a(get);
    return(0);
}
