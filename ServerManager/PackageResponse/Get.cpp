#include "Get.hpp"

Get::Get(bool isIndexing, RessourceHandler &explorer) : isIndexing(isIndexing), explorer(explorer)
{
        if (isIndexing)
                autoIndexing();
        else if (explorer.getType() == FILE_)
                readFile();
        else
                throw ErrorResponse(403, "Forbidden");
}

Get::Get( const Get &copy ) : explorer(copy.explorer)
{
        *this = copy;
}

Get&    Get::operator=(const Get &assign)
{
        if (this != &assign)
	{
		this->body = assign.body;
                this->isIndexing = assign.isIndexing;
                this->explorer = assign.explorer;
	}
	return *this;
}

Get::~Get()
{
}

String  Get::getBody( void )
{
        return body;
}

void    Get::readFile( void )
{
        std::ifstream   file(explorer.getPath().c_str(), std::ios::binary);
        if (!file.is_open())
                throw   ErrorResponse(500, "Internal Server Error");
        std::ostringstream buff;
        char buffer[4096];
        while (file.read(buffer, sizeof(buffer)))
            buff.write(buffer, file.gcount());
        buff.write(buffer, file.gcount());
        body = buff.str();
        buff.clear();
        file.close();
}

void    Get::autoIndexing( void )
{
        t_svec  directories;
        DIR   *dir = opendir(explorer.getPath().c_str());
        if (!dir)
                throw ErrorResponse(500, "Inrernal Server Error");
        struct dirent *entry;
        while ((entry = readdir(dir)))
        {
                directories.push_back(entry->d_name);
                directories.push_back(" ");
        }
        body = wsu::buildListingBody(directories);
}
