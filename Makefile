NAME			:=	webserv

CXX				:= c++  

INC_DIR		:= Inc/
SRC_DIR		:= Server/
OBJ_DIR		:= obj/

SRC_FILES	:= $(wildcard $(SRC_DIR)*.cpp)
OBJ_FILES	:= $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC_FILES))

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ_FILES) main.cpp
	$(CXX) -I$(INC_DIR) $^ -o $@
	printf "Compilación completada ->: [$@]\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) -I$(INC_DIR) -c $< -o $@
    printf "Compilando ->: {$<}\n"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	m -rf $(NAME)

fclean: clean
	rm -rf $(OBJ_DIR)

re: fclean all