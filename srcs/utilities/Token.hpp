#ifndef TOKEN_HPP
# define TOKEN_HPP

#include "BasicString.hpp"

class Token
{
	private :
		String						token;
		String						tokenId;
		std::map<String, String>	tokenDB;
		std::vector< s_body >		body;

		void						decryptData(const String &token);
		Token();

	public:
		bool						authentified(const String &id);
		String						getTokenId() const;
		void						generateTokenId();
		String						getToken() const;
		String						UserInDb() const;

		Token(const std::vector< s_body > &body, const std::map<String, String> &tokenDb);
		Token( const Token &copy );
		Token	&operator=( const Token &assign );
		~Token();
};

std::ostream &operator<<( std::ostream &o, const Token &ser );

#endif
