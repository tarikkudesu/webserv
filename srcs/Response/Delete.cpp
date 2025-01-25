#include "Delete.hpp"

Delete::Delete(RessourceHandler &explorer) : explorer(explorer)
{
      deleteFile();
}

Delete::Delete( const Delete &copy ) : explorer(copy.explorer)
{
}

Delete&    Delete::operator=(const Delete &assign)
{
        if (this != &assign)
                this->explorer = assign.explorer;
	return *this;
}

Delete::~Delete()
{
}

void    Delete::deleteFile( void )
{
        if (unlink(explorer.getPath().c_str()) != 0)
                throw ErrorResponse(500, explorer.__location, "Internal Server Error");
}
