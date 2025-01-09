SRC		=	./ServerManager/Connection.cpp \
			./ServerManager/Location.cpp \
			./ServerManager/Request.cpp \
			./ServerManager/WSU.cpp \
			./ServerManager/PackageResponse/RessourceHandler.cpp \
			./ServerManager/PackageResponse/Delete.cpp \
			./ServerManager/PackageResponse/Post.cpp \
			./ServerManager/PackageResponse/Response.cpp \
			./ServerManager/PackageResponse/Get.cpp \
			./ServerManager/PackageResponse/Cgi.cpp \
			./ServerManager/Server.cpp \
			./ServerManager/Core.cpp \
			./ServerManager/Headers.cpp \
			./ServerManager/ServerManager.cpp \
			./ServerManager/PackageResponse/ErrorResponse.cpp \
			./srcs/main.cpp

OBJ		=	$(SRC:.cpp=.o)
NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJ)
	@g++ -Wall -Wextra -Werror -std=c++98 $(OBJ) -o $(NAME)

%.o: %.cpp	./ServerManager/Location.hpp \
			./ServerManager/ServerManager.hpp \
			./ServerManager/Connection.hpp \
			./ServerManager/Request.hpp \
			./ServerManager/PackageResponse/RessourceHandler.hpp \
			./ServerManager/PackageResponse/Response.hpp \
			./ServerManager/PackageResponse/Delete.hpp \
			./ServerManager/PackageResponse/Cgi.hpp \
			./ServerManager/PackageResponse/Get.hpp \
			./ServerManager/PackageResponse/Post.hpp \
			./ServerManager/Server.hpp \
			./ServerManager/Core.hpp \
			./ServerManager/Headers.hpp \
			./ServerManager/PackageResponse/ErrorResponse.hpp \
			./ServerManager/WSU.hpp \
			./srcs/webserv.hpp
	@g++ -Wall -Wextra -Werror -std=c++98 -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
