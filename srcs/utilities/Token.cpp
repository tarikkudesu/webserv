#include "Token.hpp"

Token::Token()
{
	wsu::debug("Token Default constructor called");
}
Token::Token(const std::vector< s_body > &body, const std::map<String, String> &tokenDb) :  tokenDB(tokenDb),
                                                                                            body(body)
{
	wsu::debug("Token Default constructor called");
}

Token::Token( const Token &copy ) : token(copy.token),
									tokenId(copy.tokenId)
{
	wsu::debug("Token copy constructor called");
	(void) copy;
}

Token::~Token()
{
	wsu::debug("Token Desctructor called");
}

Token	&Token::operator=( const Token &assign )
{
	wsu::debug("Token Copy assignment operator called");
	if (this != &assign)
	{
		this->token = assign.token;
		this->tokenId = assign.tokenId;
	}
	return *this;
}

/*********************************************************************
 *                              METHODS                              *
 *********************************************************************/

String    Token::UserInDb() const
{
    char    buffer[1024];
    String userInfo;
    if (!this->body.size())
        return "";
    std::ifstream file(this->body[0]._fileName.c_str());
    while (!file.eof())
    {
        file.read(buffer, 1024);
        userInfo.append(buffer);
        bzero(buffer, 1024);
    }
    for (std::map<String, String>::const_iterator it = tokenDB.begin(); it != tokenDB.end(); it++)
    {
        if (it->second.compare(userInfo))
            return userInfo;
    }
    return "";
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
void Token::decryptData(const String &token)
{
	std::size_t pos = token.find_last_of(" ");
	if (pos == std::string::npos)
		throw std::runtime_error("Invalid token format");
	this->tokenId = token.substr(pos + 1);
	this->token = token.substr(0, pos);
}
bool Token::authentified(const String &id)
{
	if (tokenDB.find(id) != tokenDB.end())
		return true;
	return false;
}
String Token::getToken() const
{
	return token;
}
String Token::getTokenId() const
{
	return tokenId;
}
