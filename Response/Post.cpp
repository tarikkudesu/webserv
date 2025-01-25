#include "Post.hpp"

Post::Post(RessourceHandler &explorer, Request &request) : explorer(explorer), request(request)
{
  getCurr_file();
  writeFile();
}

Post::Post(const Post &copy) : explorer(copy.explorer), request(copy.request)
{
  *this = copy;
}

Post &Post::operator=(const Post &assign)
{
  if (this != &assign)
  {
    this->explorer = assign.explorer;
    this->request = assign.request;
  }
  return *this;
}

Post::~Post()
{
  for (size_t i = 0; i < request.__body.size(); i++)
  {
    unlink(request.__body[i]._fileName.c_str());
  }
}

void Post::getCurr_file(void)
{
  for (size_t i = 0; i < request.__body.size(); i++)
  {
    for (size_t j = 0; j < request.__body[i]._headers.size(); j++)
    {
      size_t Dispo_pos = request.__body[i]._headers[j].find("Content-Disposition");
      if (Dispo_pos != std::string::npos)
      {
        size_t filenamePos = request.__body[i]._headers[j].find("filename=\"", Dispo_pos);
        if (filenamePos != std::string::npos)
        {
          size_t startPos = filenamePos + 10;
          size_t endPos = request.__body[i]._headers[j].find("\"", startPos);
          if (endPos != std::string::npos)
          {
            std::string fileName = request.__body[i]._headers[j].substr(startPos, endPos - startPos);
            curr_file.push_back(fileName);
            std::cout << "Extracted file: " << fileName << std::endl;
          }
        }
      }
    }
  }
}

void Post::writeFile(void)
{
  std::vector<s_body>::iterator it1 = request.__body.begin();
  t_svec::iterator it2 = curr_file.begin();
  while (it1 != request.__body.end() && it2 != curr_file.end())
  {
    try
    {
      char buff[READ_SIZE];
      String fullPath = wsu::joinPaths("./uploads", *it2);
      std::cout << fullPath << std::endl;
      std::ifstream readFrom(it1->_fileName.c_str(), std::ios_base::binary);
      std::ofstream writeTo(fullPath.c_str());
      if (!readFrom.is_open())
        throw std::runtime_error("Can´t open file");
      if (!writeTo.is_open())
      {
        readFrom.close();
        throw std::runtime_error("Can´t open file");
      }
      while (readFrom.read(buff, READ_SIZE) || readFrom.gcount() > 0)
      {
        std::size_t bytesRead = readFrom.gcount();
        for (std::size_t i = 0; i < bytesRead; ++i)
        {
          writeTo << buff[i];
        }
      }
      readFrom.close();
      writeTo.close();
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }
    it1++;
    it2++;
  }
}
