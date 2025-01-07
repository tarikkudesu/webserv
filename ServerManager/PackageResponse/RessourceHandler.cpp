#include "./RessourceHandler.hpp"

RessourceHandler::RessourceHandler(Location &location, String &uri) : __location(location),
                                                                      __URI(uri),
                                                                      __fullPath(""),
                                                                      __type(FILE)
{
    loadPathExploring();
    loadType(__fullPath.c_str());
}

Type    RessourceHandler::loadType(const char* path)
{
    struct stat file_stat;
    if (stat(path, &file_stat) || !(file_stat.st_mode & S_IRUSR))
        throw ErrorResponse(400, "unauthorized"); // to check what the exact status code and reason phrase
    if (S_ISREG(file_stat.st_mode))
        __type = FILE;
    else if (S_ISDIR(file_stat.st_mode))
        __type = FOLDER;
}

void    RessourceHandler::loadPathExploring(void)
{
    
}

String  RessourceHandler::getPath() const
{
    return __fullPath;
}

Type    RessourceHandler::getType() const
{
    return __type;
}
