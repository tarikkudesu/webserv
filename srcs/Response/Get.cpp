#include "Get.hpp"

Get::Get(bool isIndexing, RessourceHandler &explorer) : isIndexing(isIndexing), explorer(explorer)
{
  if (explorer.getType() == FILE_)
    readFile();
  else if (isIndexing)
    autoIndexing();
  else
    throw ErrorResponse(403, "Forbidden");
}

Get::Get(const Get &copy) : explorer(copy.explorer)
{
  *this = copy;
}

Get &Get::operator=(const Get &assign)
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

String Get::getBody(void)
{
  return body;
}

void Get::readFile(void)
{
  std::ifstream file(explorer.getPath().c_str(), std::ios::binary);
  if (!file.is_open())
    throw ErrorResponse(500, explorer.__location, "Internal Server Error");
  std::ostringstream buff;
  char buffer[READ_SIZE];
  while (file.read(buffer, READ_SIZE) || file.gcount() > 0)
  {
    std::size_t bytesRead = file.gcount();
    for (std::size_t i = 0; i < bytesRead; ++i)
      buff << buffer[i];
  }
  body = buff.str();
  buff.clear();
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
  body = wsu::buildListingBody(explorer.getPath(), directories);
}
