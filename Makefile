SRC		=	./ServerManager/Config.cpp ./ServerManager/Connection.cpp ./ServerManager/ErrorResponse.cpp ./ServerManager/Location.cpp ./ServerManager/Logs.cpp ./ServerManager/Request.cpp ./ServerManager/Response.cpp ./ServerManager/Server.cpp ./ServerManager/ServerManager.cpp ./srcs/main.cpp 
OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJ)
	@c++ -std=c++98 $(OBJ) -o $(NAME)

%.o: %.cpp ./ServerManager/Config.hpp ./ServerManager/Connection.hpp ./ServerManager/ErrorResponse.hpp ./ServerManager/Location.hpp ./ServerManager/Logs.hpp ./ServerManager/Request.hpp ./ServerManager/Response.hpp ./ServerManager/Server.hpp ./ServerManager/ServerManager.hpp ./srcs/webserv.hpp 
	@c++ -std=c++98 -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
