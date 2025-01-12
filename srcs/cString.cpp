#include "cString.hpp"

cString::cString() : __buff(NULL), __size(0)
{
}

cString::cString(char *buff, size_t size) : __buff(NULL), __size(size)
{
	if (buff == NULL)
		return ;
	__buff = new char[__size];
	for (size_t i = 0; i < __size; i++)
		__buff[i] = buff[i];
}

cString::cString(const cString &copy) : __buff(NULL), __size(0)
{
	if (copy.__size == 0)
		return;
	__size = copy.__size;
	__buff = new char[__size];
	for (size_t i = 0; i < __size; i++)
		__buff[i] = copy.__buff[i];
}

cString::~cString()
{
	delete[] __buff;
}

cString &cString::operator=(const cString &assign)
{
	if (this != &assign)
	{
		delete[] __buff;
		__size = assign.__size;
		if (__size != 0)
		{
			__buff = new char[__size];
			for (size_t i = 0; i < __size; i++)
				__buff[i] = assign.__buff[i];
		}
	}
	return *this;
}
void cString::erase(size_t start, size_t end)
{
	if (start >= __size || end >= __size || start > end)
		return;
	size_t len_to_remove = end - start;
	size_t new_size = __size - len_to_remove;
	char *new_buff = new char[new_size];
	for (size_t i = end; i < __size; i++)
		new_buff[i - len_to_remove] = __buff[i];
	delete[] __buff;
	__buff = new_buff;
	__size = new_size;
}
void cString::join(const cString &j)
{
	char *new_buff = new char[__size + j.__size];
	for (size_t i = 0; i < __size; i++)
		new_buff[i] = __buff[i];
	for (size_t i = 0; i < j.__size; i++)
		new_buff[__size + i] = j.__buff[i];
	delete[] __buff;
	__buff = new_buff;
	__size = __size + j.__size;
}
char *cString::getBuff() const
{
	return __buff;
}
size_t cString::length() const
{
	return __size;
}
cString cString::duplicate()
{
	char *new_buff = new char[__size];
	for (size_t i = 0; i < __size; i++)
		new_buff[i] = __buff[i];
	cString tmp(new_buff, __size);
	delete[] new_buff;
	return tmp;
}

cString cString::substr(size_t start, size_t length)
{
	if (start >= __size)
		return cString(NULL, 0);
	if (start + length > __size)
		length = __size - start;
	char *new_buff = new char[length];
	for (size_t i = 0; i < length; i++)
		new_buff[i] = __buff[start + i];
	return cString(new_buff, length);
}
std::string cString::to_string() const
{
	std::string result;
	result.reserve(__size);
	for (size_t i = 0; i < __size; i++)
		result += __buff[i];
	return result;
}
size_t cString::find(const String &f) const
{
	if (f.length() == 0 || f.length() > this->__size)
		return String::npos;
	for (size_t i = 0; i <= __size - f.length(); i++)
	{
		bool found = true;
		for (size_t j = 0; j < f.length(); j++)
		{
			if (__buff[i + j] != f[j])
			{
				found = false;
				break;
			}
		}
		if (found)
			return i;
	}
	return String::npos;
}
std::ostream &operator<<(std::ostream &out, const cString &str)
{
	char *buff = str.getBuff();
	size_t size = str.length();
	for (size_t i = 0; i < size; i++)
		out << buff[i];
	return out;
}