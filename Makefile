SRC		=	ServerManager/Core.cpp \
			ServerManager/Location.cpp \
			ServerManager/Server.cpp \
			ServerManager/ServerManager.cpp \
			Response/RessourceHandler.cpp \
			Response/Delete.cpp \
			Response/Post.cpp \
			Response/Response.cpp \
			Response/Get.cpp \
			Response/ErrorResponse.cpp \
			Response/Cgi.cpp \
			srcs/WSU.cpp \
			srcs/main.cpp \
			Request/Connection.cpp \
			Request/Headers.cpp \
			Request/Request.cpp
OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJ)
	@c++ -Wall -Wextra -Werror -g -fsanitize=address -std=c++98 $(OBJ) -o $(NAME)

%.o: %.cpp	ServerManager/Location.hpp \
			ServerManager/ServerManager.hpp \
			ServerManager/Server.hpp \
			ServerManager/Core.hpp \
			Response/RessourceHandler.hpp \
			Response/Response.hpp \
			Response/ErrorResponse.hpp \
			Response/Delete.hpp \
			Response/Cgi.hpp \
			Response/Get.hpp \
			Response/Post.hpp \
			srcs/WSU.hpp \
			srcs/webserv.hpp \
			Request/Connection.hpp \
			Request/Request.hpp \
			Request/Headers.hpp
	@c++ -Wall -Wextra -Werror -g -fsanitize=address -std=c++98 -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean