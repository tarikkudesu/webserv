SRC		=	./ServerManager/ErrorResponse.cpp \
			./ServerManager/Location.cpp \
			./ServerManager/Request.cpp \
			./ServerManager/Response.cpp \
			./ServerManager/Server.cpp \
			./ServerManager/ServerManager.cpp \
			./ServerManager/Cgi.cpp \
			./ServerManager/Connection.cpp \
			./ServerManager/WSU.cpp \
			./srcs/main.cpp

OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJ)
	@c++ -Wall -Wextra -Werror -g -fsanitize=address -std=c++98 $(OBJ) -o $(NAME)

%.o: %.cpp	./ServerManager/ErrorResponse.hpp \
			./ServerManager/ServerManager.hpp \
			./ServerManager/Cgi.hpp \
			./ServerManager/WSU.hpp \
			./ServerManager/Connection.hpp \
			./ServerManager/Location.hpp \
			./ServerManager/Request.hpp \
			./ServerManager/Response.hpp \
			./ServerManager/Server.hpp \
			./srcs/webserv.hpp
	@c++ -Wall -Wextra -Werror -g -std=c++98 -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean



