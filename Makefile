SRC		=	ServerManager/Cgi.cpp \
			ServerManager/Connection.cpp \
			ServerManager/Core.cpp \
			ServerManager/Delete.cpp \
			ServerManager/ErrorResponse.cpp \
			ServerManager/Get.cpp \
			ServerManager/Headers.cpp \
			ServerManager/Location.cpp \
			ServerManager/Post.cpp \
			ServerManager/Request.cpp \
			ServerManager/Response.cpp \
			ServerManager/RessourceHandler.cpp \
			ServerManager/Server.cpp \
			ServerManager/ServerManager.cpp \
			ServerManager/WSU.cpp \
			srcs/main.cpp 
OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJ)
	@c++ -Wall -Wextra -Werror -std=c++98 $(OBJ) -o $(NAME)

%.o: %.cpp	ServerManager/Cgi.hpp \
			ServerManager/Connection.hpp \
			ServerManager/Core.hpp \
			ServerManager/Delete.hpp \
			ServerManager/ErrorResponse.hpp \
			ServerManager/Get.hpp \
			ServerManager/Headers.hpp \
			ServerManager/Location.hpp \
			ServerManager/Post.hpp \
			ServerManager/Request.hpp \
			ServerManager/Response.hpp \
			ServerManager/RessourceHandler.hpp \
			ServerManager/Server.hpp \
			ServerManager/ServerManager.hpp \
			ServerManager/WSU.hpp \
			srcs/webserv.hpp 
	@c++ -Wall -Wextra -Werror -std=c++98 -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
