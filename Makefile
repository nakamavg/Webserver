CC = c++
CFLAGS =  
RM = rm -rf
NAME = testeo

SRCS = srcs/ServerConfig.cpp\
	   srcs/WebServer.cpp\
	   srcs/ParseUtils.cpp\
	   srcs/ParseRequest.cpp\
	   srcs/ServerUp.cpp\
	   srcs/Cgi.cpp\
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
