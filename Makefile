# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anurtiag <anurtiag@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/17 10:54:22 by anurtiag          #+#    #+#              #
#    Updated: 2024/10/02 13:13:54 by anurtiag         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	webserv

SRCS			=	pruebas.cpp


OBJ				=	$(SRCS:.cpp=.o)


CC				=	c++


CFLAGS			=	 -Wall -Wextra -Werror


RM				=	rm -rf


all:			$(NAME)


$(NAME):		$(OBJ)
				@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)


%.o:			%.cpp
				$(CC) $(CFLAGS) -c $< -o $@


clean:
				@$(RM) $(OBJ)


fclean:			clean
				@$(RM) $(NAME)


re:				fclean all


.PHONY:			all clean fclean re