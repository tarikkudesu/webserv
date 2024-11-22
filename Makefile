SRC		=	./ServerManager/Config/Config.cpp ./ServerManager/Logs/Logs.cpp ./ServerManager/Server/Server.cpp ./ServerManager/ServerEngine.cpp ./srcs/main.cpp 
OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJ)
	@c++ -Wall -Wextra -Werror -std=c++98 $(OBJ) -o $(NAME)

%.o: %.cpp ./ServerManager/Config/Config.hpp ./ServerManager/Logs/Logs.hpp ./ServerManager/Server/Server.hpp ./ServerManager/ServerEngine.hpp ./srcs/webserv.hpp 
	@c++ -Wall -Wextra -Werror -std=c++98 -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
