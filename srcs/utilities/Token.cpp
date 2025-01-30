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

Token::Token( const Token &copy ) : tokenDB(copy.tokenDB),
									body(copy.body)
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
		this->tokenDB = assign.tokenDB;
		this->body = assign.body;
	}
	return *this;
}

/*********************************************************************
 *                              METHODS                              *
 *********************************************************************/

String    Token::addUserInDb(String userInfo, String serverFile)
{
	String cookie("");
	std::ofstream file( ("essentials/" + serverFile).c_str() ,std::ios::app); // Open file for appending

	if (file.is_open())
	{
		cookie = generateTokenId();
		file << userInfo + " " + cookie  << "\n";
		file.close();
	}
	return cookie;
}

String	Token::getCookie(String& id)
{
	std::map<String, String>::iterator element = this->tokenDB.find(id);
	if (element == tokenDB.end())
		return "";
	return element->second;
}

String Token::generateTokenId()
{
	String tokenId;
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
	return tokenId;
}

bool Token::authentified(const String &id)
{
	if (tokenDB.find(id) != tokenDB.end())
		return true;
	return false;
}


