#include "string.h"
#include "memory.h"
#include <iostream>

namespace mystd {

class string::out_of_range_exception
{
	
};

const unsigned int string::no_match = -1;
const unsigned int string::end_of_str = -1;

string::string()
{
	str_len = 0;
	str = nullptr;
}

string::string(const char* in_str)
{
	str_len = 0;
	str = nullptr;
	attach(in_str);
}

string::string(const string& copy)
{
	str_len = 0;
	str = nullptr;
	attach(copy.out_str);
}

string::~string() noexcept
{
	delete[] str;
}

string& string::operator= (const char* in_str)
{
	unsigned char* temporary_pointer = str;
	str = nullptr;
	attach(in_str);
	delete[] temporary_pointer;
	return *this;
}

string& string::operator= (const string& in_string)
{
	unsigned char* temporary_pointer = str;
	str = nullptr;
	attach(in_string.out_str);
	delete[] temporary_pointer;
	return *this;
}

string& string::operator+= (const char* in_str)
{
	attach(in_str);
	return *this;
}

string& string::operator+= (const string& in_string)
{
	attach(in_string.out_str);
	return *this;
}

string operator+ (const string& lhs, const string& rhs)
{
	string result(lhs.out_str);
	result.add(rhs.out_str);
	return result;
}

string operator+ (const string& lhs, const char* rhs)
{
	string result(lhs.out_str);
	result.add(rhs);
	return result;
}

string operator+ (const char* lhs, const string& rhs)
{
	string result(lhs);
	result.add(rhs.out_str);
	return result;
}

unsigned char& string::operator[] (unsigned int pos)
{
	if(pos < str_len)
	{
		return str[pos];
	}
	else
	{
		throw out_of_range_exception();
	}
}

char string::operator[] (unsigned int pos) const
{
	if(pos < str_len)
	{
		return str[pos];
	}
	else
	{
		throw out_of_range_exception();
	}
}

unsigned char* string::m_alloc(unsigned int size) const
{
	return new unsigned char[size];
}

unsigned char* string::add(const char* in_str)
{
	attach(in_str);
	return str;
}

unsigned char* string::add(const string& in_string)
{
	attach(in_string.out_str);
	return str;
}


char string::at(unsigned int pos) const 
{
	if(pos < str_len)
	{
		return str[pos];
	}
	else
	{
		throw out_of_range_exception();
	}
}

unsigned char* string::remove(unsigned int start_pos, unsigned int end_pos)
{
	string sub;
	
	if(start_pos > end_pos)
	{
		throw out_of_range_exception();
	}
	else if(end_pos > str_len-2 && end_pos != end_of_str)
	{
		throw out_of_range_exception();
	}
	else if(start_pos == 0 && (end_pos == str_len-2 || end_pos == end_of_str))
	{
		sub = "";
	}
	else if(start_pos == 0 && end_pos != end_of_str) 
	{
		sub = substr(end_pos+1, str_len-2);
	}
	else if(end_pos == str_len-2 || end_pos == end_of_str)
	{
		sub = substr(0, start_pos-1);
	}
	else
	{
		sub = substr(0, start_pos-1);
		sub.add(substr(end_pos+1, str_len-2));
	}
	
	delete[] str;
	str = nullptr;
	attach(sub.out_str);
	return str;
}

unsigned char* string::attach(const char* in_str)
{
	unsigned int in_str_len = 0;
	
	for(unsigned int counter = 0; in_str[counter] != '\0'; counter++)
	{
		++in_str_len;
	}
	++in_str_len;
	
	if(str == nullptr)
	{
		str = m_alloc(in_str_len);
		out_str = (const char*)str;
		mystd::memcpy<char>(str, in_str, in_str_len);
		str_len = in_str_len;
	}
	else 
	{
		unsigned char* _ptr = m_alloc((in_str_len + str_len)-1);
		mystd::memcpy<char>(_ptr, str, str_len-1);
		mystd::memcpy<char>(_ptr+str_len-1, in_str, in_str_len);
		delete[] str;
		str = _ptr;
		out_str = (const char*)str;
		str_len = (str_len + in_str_len)-1;
	}
	return str;
}

unsigned char* string::begin()
{
	if(str != nullptr)
	{
		return str;
	}
	else
	{
		throw out_of_range_exception();
	}
}

unsigned char* string::end()
{
	if(str != nullptr)
	{
		return &str[str_len-1];
	}
	else
	{
		throw out_of_range_exception();
	}
}

const char* string::out() const
{
	if(str != nullptr)
	{
		return out_str;
	}
	else
	{
		return "";
	}
}

unsigned int string::length() const
{
	return str_len;
}

unsigned int string::find(const char* in_str) const
{
	unsigned int pos = -1;
	unsigned int in_str_len = 0;
	bool found = false;
	
	for(unsigned int counter = 0; in_str[counter] != '\0'; counter++)
	{
		++in_str_len;
	}
	
	if(str_len < in_str_len)
	{
		return pos;
	}
	
	for(unsigned int counter = 0; counter < str_len-in_str_len; counter++)
	{
		if(found == false)
		{
			for(unsigned int inner_counter = 0; 
				inner_counter < in_str_len;
				inner_counter++)
			{
				if(str[counter+inner_counter] != in_str[inner_counter])
				{
					break;
				}
				else if (inner_counter == in_str_len-1)
				{
					pos = counter;
					found = true;
				}
			}
		}
		else
		{
			break;
		}
		
	}
	return pos;
}

unsigned int string::find(const string& in_string) const
{
	return find(in_string.out_str);
}

string string::substr(unsigned int start_pos, unsigned int end_pos) const
{
	string sub;
	
	if(end_pos == end_of_str)
	{
		end_pos = str_len-2;
	}
	
	unsigned int range = end_pos - start_pos;
	unsigned char* n_mem_array = nullptr;
	
	if((start_pos < str_len - 1)
		&& (end_pos < str_len - 1) 
		&& (start_pos >= 0) 
		&& (end_pos >= 0)
		&& (start_pos <= end_pos))
	{
		n_mem_array = m_alloc(range + 2);
		for(unsigned int counter=0; counter <= range; counter++)
		{
			n_mem_array[counter] = str[start_pos+counter];
		}
		//gs_memcpy<char>(n_mem_array, &str[start_pos], range+1);
		n_mem_array[range+1] = '\0';
		sub.add((const char*)n_mem_array);
	}
	else
	{
		throw out_of_range_exception();
	}
	return sub;
}

} // namespace