SRC		=	Request/Connection.cpp \
			Request/Headers.cpp \
			Request/Request.cpp \
			Response/Cgi.cpp \
			Response/Get.cpp \
			Response/Post.cpp \
			Response/Delete.cpp \
			Response/Response.cpp \
			Response/ErrorResponse.cpp \
			Response/RessourceHandler.cpp \
			ServerManager/Core.cpp \
			ServerManager/Server.cpp \
			ServerManager/Location.cpp \
			ServerManager/ServerManager.cpp \
			srcs/WSU.cpp \
			srcs/main.cpp \
			srcs/BasicString.cpp

OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: mkdir $(NAME)

$(NAME): $(OBJ)
	@g++ -Wall -Wextra -Werror -std=c++98 $(OBJ) -o $(NAME)

%.o: %.cpp	Request/Connection.hpp \
			Request/Headers.hpp \
			Request/Request.hpp \
			Response/Cgi.hpp \
			Response/Get.hpp \
			Response/Post.hpp \
			Response/Delete.hpp \
			Response/Response.hpp \
			Response/ErrorResponse.hpp \
			Response/RessourceHandler.hpp \
			ServerManager/Core.hpp \
			ServerManager/Server.hpp \
			ServerManager/Location.hpp \
			ServerManager/ServerManager.hpp \
			srcs/WSU.hpp \
			srcs/webserv.hpp \
			srcs/BasicString.hpp
	@c++ -Wall -Wextra -Werror -std=c++98 -c $< -o $@

mkdir:
	@mkdir -p .temp

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean mkdir
