#include "Token.hpp"

Token::Token(Request &request) : request(request)
{
    generateTokenId();
}

void Token::encryptData()
{
    std::ifstream f(request.__body[0]._fileName);
    if (!f.is_open())
        throw ErrorResponse(500, "Internal Server Error");
    std::ostringstream buffer;
    buffer << f.rdbuf();
    token = buffer.str();
    token += tokenId;
    f.close();
}

void Token::generateTokenId()
{
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string specialChars = "123456789";
    static int sed = 0;
    for (int i = 0; i < 10; i++)
    {
        std::srand(static_cast<unsigned int>(std::time(0) + sed++));
        int randomAlphabetIndex = std::rand() % alphabet.size();
        int randomSpecialIndex = std::rand() % specialChars.size();
        tokenId += alphabet[randomAlphabetIndex];
        tokenId += specialChars[randomSpecialIndex];
    }
}

void Token::decryptData()
{
}

bool Token::authentified(String id)
{
}

String Token::getToken() const
{
    return token;
}

String Token::getTokenId() const
{
    return tokenId;
}
