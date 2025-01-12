SRC		=	./Request/Connection.cpp ./Request/Headers.cpp ./Request/Request.cpp ./Response/Cgi.cpp ./Response/Delete.cpp ./Response/ErrorResponse.cpp ./Response/Get.cpp ./Response/Post.cpp ./Response/Response.cpp ./Response/RessourceHandler.cpp ./ServerManager/Core.cpp ./ServerManager/Location.cpp ./ServerManager/Server.cpp ./ServerManager/ServerManager.cpp ./srcs/cString.cpp ./srcs/main.cpp ./srcs/WSU.cpp 
OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJ)
	@c++ -Wall -Wextra -Werror -std=c++98 $(OBJ) -o $(NAME)

%.o: %.cpp ./Request/Connection.hpp ./Request/Headers.hpp ./Request/Request.hpp ./Response/Cgi.hpp ./Response/Delete.hpp ./Response/ErrorResponse.hpp ./Response/Get.hpp ./Response/Post.hpp ./Response/Response.hpp ./Response/RessourceHandler.hpp ./ServerManager/Core.hpp ./ServerManager/Location.hpp ./ServerManager/Server.hpp ./ServerManager/ServerManager.hpp ./srcs/cString.hpp ./srcs/webserv.hpp ./srcs/WSU.hpp 
	@c++ -Wall -Wextra -Werror -std=c++98 -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
