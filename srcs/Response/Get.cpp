#include "Get.hpp"

Get::Get(bool isIndexing, RessourceHandler &explorer, std::vector<BasicString> &body) : isIndexing(isIndexing),
                                                                                        explorer(explorer),
                                                                                        body(body)
{
    wsu::debug("Get constructor");
    if (explorer.getType() == FILE_)
        readFile();
    else if (isIndexing)
        autoIndexing();
    else
        throw ErrorResponse(403, "Forbidden");
}

Get::Get(const Get &copy) : isIndexing(copy.isIndexing),
                            explorer(copy.explorer),
                            body(copy.body)
{
    *this = copy;
}

Get &Get::operator=(const Get &assign)
{
    if (this != &assign)
    {
        this->body = assign.body;
        this->explorer = assign.explorer;
        this->isIndexing = assign.isIndexing;
    }
    return *this;
}

Get::~Get()
{
}

void Get::readFile(void)
{
    std::ifstream file(explorer.getPath().c_str(), std::ios::binary);
    if (!file.is_open())
        throw ErrorResponse(500, explorer.__location, "Internal Server Error");
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
        this->body.push_back(BasicString(buffer, file.gcount()));
    file.close();
}

void Get::autoIndexing(void)
{
    t_svec directories;
    DIR *dir = opendir(explorer.getPath().c_str());
    if (!dir)
        throw ErrorResponse(500, explorer.__location, "could not open directory");
    struct dirent *entry;
    while ((entry = readdir(dir)))
    {
        directories.push_back(entry->d_name);
        directories.push_back(" ");
    }
    closedir(dir);
    body.push_back(wsu::buildListingBody(explorer.getPath(), directories));
}
