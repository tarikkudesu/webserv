SRC		=	ServerManager/Connection.cpp \
			ServerManager/ErrorResponse.cpp \
			ServerManager/Location.cpp \
			ServerManager/WSU.cpp \
			ServerManager/Request.cpp \
			ServerManager/Response.cpp \
			ServerManager/Server.cpp \
			ServerManager/ServerManager.cpp \
			srcs/main.cpp 

OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJ)
	@c++ -Wall -Wextra -Werror $(OBJ) -o $(NAME)

%.o: %.cpp	ServerManager/Connection.hpp \
			ServerManager/ErrorResponse.hpp \
			ServerManager/Location.hpp \
			ServerManager/WSU.hpp \
			ServerManager/Request.hpp \
			ServerManager/Response.hpp \
			ServerManager/Server.hpp \
			ServerManager/ServerManager.hpp \
			srcs/webserv.hpp 
	@c++ -Wall -Wextra -Werror -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
