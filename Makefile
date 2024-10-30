# Definir colores
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No color (reiniciar)

# Comandos
CC = c++
CCFLAGS = -Wall -Wextra -Werror -std=c++98   

# Nombre ejecutable
NAME = webserv

# Ficheros
SRC_FILES = ServerUp\
	   Cgi\
	   Response\
	   ServerConfig\
	   WebServer\
	   ParseUtils\
	   ParseRequest\
	   main

SRC = $(addprefix srcs/, $(addsuffix .cpp, $(SRC_FILES)))
OBJ = $(addprefix objs/, $(addsuffix .o, $(SRC_FILES)))

# REGLAS # 
all: $(NAME)

# Compilar 
$(NAME): $(OBJ)
		@$(CC) $(OBJ) $(CCFLAGS) -o $(NAME)
		@echo "$(GREEN)WEBSERVER HAS BEEN COMPILED!$(NC)"

objs/%.o: srcs/%.cpp
	@mkdir -p objs
	@echo "$(YELLOW)Compiling: $<$(NC)"
	@$(CC) $(CCFLAGS) -o $@ -c $<
	@echo "$(YELLOW)Compiled!$(NC)"

# Eliminar temporales
clean:
	@rm -rf objs/
	@echo "$(RED)OBJS AND DIRECTORY CLEANED!$(NC)"

# Eliminar temporales y ejecutable
fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)EXECUTABLE CLEANED!$(NC)"

re: fclean all
