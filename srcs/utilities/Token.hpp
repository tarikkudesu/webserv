#ifndef TOKEN_HPP
# define TOKEN_HPP

#include "BasicString.hpp"

class Token
{
	private :
		std::map<String, String>	tokenDB;
		std::vector< s_body >		body;
		Token();

	public:
		bool						authentified(const String &id);
		String						generateTokenId();
		String						getCookie(String& id);
		String						addUserInDb(String userInfo, String serverFile);

		Token(const std::vector< s_body > &body, const std::map<String, String> &tokenDb);
		Token( const Token &copy );
		Token	&operator=( const Token &assign );
		~Token();
};

#endif
