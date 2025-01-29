#ifndef BasicString_HPP
#define BasicString_HPP

#include "WSU.hpp"

class BasicString
{
private:
	char *__buff;
	size_t __size;

public:
	BasicString substr(size_t start, size_t length);
	void erase(size_t start, size_t end);
	size_t find(const String &f) const;
	void join(const BasicString &j);
	BasicString duplicate() const;
	std::string to_string() const;
	char *getBuff() const;
	size_t length() const;
	void clear();

	BasicString();
	BasicString(const String &str);
	BasicString(char *buff, size_t size);
	BasicString(const BasicString &copy);
	BasicString &operator=(const BasicString &assign);
	~BasicString();
};

std::ostream &operator<<(std::ostream &out, const BasicString &str);

#endif
