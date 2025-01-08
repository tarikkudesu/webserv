#include "Post.hpp"

Post::Post(RessourceHandler &explorer, String requestbody) : explorer(explorer), __requestbody(requestbody)
{
        std::cout << RED << "POST\n" << RESET;
        writeFile();
}

Post::Post( const Post &copy ) : explorer(copy.explorer)
{
        *this = copy;
}

Post&    Post::operator=(const Post &assign)
{
        if (this != &assign)
        {
                this->explorer = assign.explorer;
                this->__requestbody = assign.__requestbody;
        }
	return *this;
}

Post::~Post()
{
}

void    Post::writeFile( void )
{
        String path = wsu::joinPaths("uploads/", explorer.getURI());
        std::ofstream   file;

        file.open(path.c_str());
        if (!file)
                throw   ErrorResponse(500, explorer.__location, "Internal Server Error");
        file << __requestbody;
        if (file.fail()) {
                throw ErrorResponse(500, explorer.__location, "Internal Server Error: Failed to write to file " + path);
    }
}
