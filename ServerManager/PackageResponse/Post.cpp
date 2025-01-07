#include "Post.hpp"

Post::Post(RessourceHandler &explorer, String requestbody) : explorer(explorer), __requestbody(requestbody)
{
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
        String path = wsu::joinPaths("/uploads", explorer.getPath());
        std::ofstream   file(path.c_str());

        if (!file.is_open())
                throw   ErrorResponse(500, "Internal Server Error");
        file << __requestbody;
        if (file.fail()) {
                throw ErrorResponse(500, "Internal Server Error: Failed to write to file " + path);
    }
}
