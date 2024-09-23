# Variables
CXX = c++
CXXFLAGS = -std=c++98 -I./Incl

# Archivos
SRCS = Server/Server.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = server

# Reglas
all:	$(EXEC)

$(EXEC):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)