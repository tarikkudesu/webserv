SRC		=	srcs/main.cpp \
			srcs/Request/Connection.cpp \
			srcs/Request/Headers.cpp \
			srcs/Request/Request.cpp \
			srcs/Response/Cgi.cpp \
			srcs/Response/Delete.cpp \
			srcs/Response/ErrorResponse.cpp \
			srcs/Response/Get.cpp \
			srcs/Response/Post.cpp \
			srcs/Response/Response.cpp \
			srcs/Response/RessourceHandler.cpp \
			srcs/ServerManager/Core.cpp \
			srcs/ServerManager/Location.cpp \
			srcs/ServerManager/Server.cpp \
			srcs/ServerManager/ServerManager.cpp \
			srcs/utilities/BasicString.cpp \
			srcs/utilities/Token.cpp \
			srcs/utilities/WSU.cpp 
OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: mkdir $(NAME)

$(NAME): $(OBJ)
	@c++ -Wall -Wextra -Werror -std=c++98 $(OBJ) -o $(NAME) -g -fsanitize=address

%.o: %.cpp	srcs/Request/Connection.hpp \
			srcs/Request/Headers.hpp \
			srcs/Request/Request.hpp \
			srcs/Response/Cgi.hpp \
			srcs/Response/Delete.hpp \
			srcs/Response/ErrorResponse.hpp \
			srcs/Response/Get.hpp \
			srcs/Response/Post.hpp \
			srcs/Response/Response.hpp \
			srcs/Response/RessourceHandler.hpp \
			srcs/ServerManager/Core.hpp \
			srcs/ServerManager/Location.hpp \
			srcs/ServerManager/Server.hpp \
			srcs/ServerManager/ServerManager.hpp \
			srcs/utilities/BasicString.hpp \
			srcs/utilities/Token.hpp \
			srcs/utilities/WSU.hpp \
			srcs/webserv.hpp 
	@c++ -Wall -Wextra -Werror -std=c++98 -c $< -o $@

mkdir:
	@mkdir -p .temp
	@mkdir -p .logs
	@mkdir -p uploads

rmdir:
	@rm -rf .temp
	@rm -rf .logs
	@rm -rf uploads

clean:
	@rm -f $(OBJ)

fclean: clean rmdir
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
