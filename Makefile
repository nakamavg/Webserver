<<<<<<< HEAD
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
=======
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 
RM = rm -rf
NAME = testeo

SRCS = srcs/ServerConfig.cpp\
	   srcs/WebServer.cpp\
	   srcs/ParseUtils.cpp\
	   main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	${CC} ${CFLAGS} -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: all clean
	./$(NAME)

.PHONY: all clean fclean re run
>>>>>>> 1d3417d0a0981cb860465ab8f9e0b7bc381567ea
