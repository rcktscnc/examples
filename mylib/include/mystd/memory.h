#ifndef __MYSTD_TOOLS_H
#define __MYSTD_GSTOOLS_H

namespace mystd {

template<typename T>
void* memcpy(void* destination, const void* source, const unsigned int elements)
{
	if(elements)
	{
		const unsigned int type_size = sizeof(T);
		unsigned char* c_destination = (unsigned char*)destination;
		const unsigned char* c_source = (const unsigned char*)source;
		
		for(unsigned int counter = 0; counter < type_size*elements; counter++)
		{
			c_destination[counter] = c_source[counter];
		}
	}
	return destination;
}

template<typename T>
void* memset(void* destination, const void* source, const unsigned int elements)
{
	if(elements)
	{
		const unsigned int type_size = sizeof(T);
		unsigned char* c_destination = (unsigned char*)destination;
		const unsigned char* c_source = (const unsigned char*)source;
		unsigned int offset = 0;
		
		for(unsigned int counter = 0; counter < elements; counter++)
		{
			for(unsigned int inner_counter = 0; inner_counter < type_size; inner_counter++)
			{
				c_destination[offset] = c_source[inner_counter];
				++offset;
			}
		}
	}
	
	return destination;
}

//-----------------------------------------------------

template<typename T>
void callback(const unsigned int repeat, T (*call_b)())
{
	for(unsigned int counter = 0; counter < repeat; counter++)
	{
		call_b();
	}
}

//-----------------------------------------------------

template <typename T>
class Range
{
	private:
	T* mCollection;
	unsigned int mSize;
	
	public:
	Range(T* collection, unsigned int size) :
		mCollection(collection), mSize(size)
	{
	}

	T* begin() { return &mCollection[0]; }
	T* end () { return &mCollection[mSize]; }
};

}
#endif