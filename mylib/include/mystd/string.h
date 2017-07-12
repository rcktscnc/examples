/* 1 December 2015
 * 
 * 
 * 
 * 
 * 
 */

#ifndef __MYSTD_STRING_H
#define __MYSTD_STRING_H


namespace mystd {

class string
{
private:
	unsigned int str_len;
	unsigned char* str;
	const char* out_str;
	unsigned char* m_alloc(unsigned int size) const;
	unsigned char* attach(const char* in_str);
	class out_of_range_exception;
	
public:
	static const unsigned int no_match;
	static const unsigned int end_of_str;
	string();
	string(const char* in_str);
	string(const string& copy);
	~string() noexcept;
	string& operator= (const char* in_str);
	string& operator= (const string& in_string);
	string& operator+= (const char* in_str);
	string& operator+= (const string& in_string);
	friend string operator+ (const string& lhs, const string& rhs);
	friend string operator+ (const string& lhs, const char* rhs);
	friend string operator+ (const char* lhs, const string& rhs);
	unsigned char& operator[] (unsigned int pos);
	char operator[] (unsigned int pos) const;
	unsigned char* add(const char* in_str);
	unsigned char* add(const string& in_string);
	char at(unsigned int pos) const;
	unsigned char* remove(unsigned int start_pos, unsigned int end_pos);
	unsigned char* begin();
	unsigned char* end();
	const char* out() const;
	unsigned int length() const;
	unsigned int find(const char* in_str) const;
	unsigned int find(const string& in_string) const;
	string substr(unsigned int start_pos, unsigned int end_pos) const;
};

} // namespace

#endif