#include "./RessourceHandler.hpp"

RessourceHandler::RessourceHandler(Location &location, String &uri) : __location(location),
                                                                      __URI(uri),
                                                                      __fullPath(""),
                                                                      __type(FILE_)
{
    loadPathExploring();
}

void    RessourceHandler::loadType(const char* path)
{
    struct stat file_stat;
    if (stat(path, &file_stat) || !(file_stat.st_mode & S_IRUSR))
        throw ErrorResponse(400, "unauthorized"); // to check what the exact status code and reason phrase
    if (S_ISREG(file_stat.st_mode))
        __type = FILE_;
    else if (S_ISDIR(file_stat.st_mode))
        __type = FOLDER;
    else
        throw ErrorResponse(404, "Ressource not found");
}

void    RessourceHandler::loadPathExploring(void)
{
    __fullPath = wsu::joinPaths(__location.__root, __URI);
    loadType(__fullPath.c_str());
    if (__type == FOLDER)
    {
        for (t_svec::iterator it = __location.__index.begin(); it != __location.__index.end(); ++it)
        {
            String  s = wsu::joinPaths(__fullPath, *it);
            if (!access(s.c_str(), F_OK))
            {
                __fullPath = s;
                return;
            }
        }
    }
}

String  RessourceHandler::getPath() const
{
    return __fullPath;
}

t_type    RessourceHandler::getType() const
{
    return __type;
}
