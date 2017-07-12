/* 1 December 2015
 * 
 * TODO LIST:
 * 1 - rename similar functions to make them overloaded functions 
 *     (example: insert_range and insert_single should become 'insert').
 * 
 * 2 - consider removing deprecated functions (these functions are marked in
 *     class prototype).
 * 
 * 3 - store non-default constructible class objects.
 * 
 * 4 - use move semantics in copy constructor or copy assignment operator
 * 
 * 5 - 2 - use initializer list to add multiple elements during container
 *     construction (example: "Container<int> c = {1, 2, 3};").
 * 
 * What this container CANNOT do:
 * 1 - store non-default constructible class objects.
 * 
 * 2 - use initializer list to add multiple elements during container
 *     construction (example: "Container<int> c = {1, 2, 3};").
 * 
 * 3 - use move semantics in copy constructor or copy assignment operator
 * 
 */

#ifndef _MYSTD_CONTAINER_H
#define _MYSTD_CONTAINER_H
#include <iostream>

namespace mystd {

template<typename T>
class container
{
private:
	T* object;
	int array_size;
	int array_iterator;
	bool m_alloc(const int size);
	bool removal_logic(const int remove_start, const int remove_end);
	
public:
	class out_of_range_exception;
	container<T>& operator= (const container<T>& copy) noexcept;
	T& operator[] (int object_location);
	T operator[] (int object_location) const;
	container(); 
	container(const int size);
	container(const int size, const T& value);
	container(const T* start_address, const T* end_address);
	container(const container<T>& copy) noexcept;
	~container();
	bool add(const T& in_object);
	bool add(const container<T>& in_container);
	T* addr(const int object_location);
	T* begin();
	int first_element_position() const; // DEPRECATED? THINK ABOUT IT!
	int capacity() const;
	bool clear();
	bool empty() const;
	T* end();
	int last_element_position() const; // DEPRECATED? THINK ABOUT IT!
	T get(const int object_location) const;
	bool insert_single(const int object_location, const T& in_object);
	bool insert_range(const int object_location, const container<T>& in_container);
	bool pop();
	container<T> range(const int start, const int end) const;
	bool remove_single(const int object_location);
	bool remove_range(const int start, const int end);
	bool replace(const int object_location, const T& in_object);
	bool shrink();
	int size() const;
};

/* ****************************************************************************************************************************
 *  										DEFINITION OF THE EXCEPTION CLASSES
 * ****************************************************************************************************************************/

template<typename T> class container<T>::out_of_range_exception {
	
};

/* ****************************************************************************************************************************
 *  										DEFINITION OF THE PRIVATE FUNCTIONS
 * ****************************************************************************************************************************/

// m_alloc() generates a new memory block, copies the content to it, deletes the old block and points 
// "object" to the new memory block. It always allocates roughly an extra 25% of memory, which prevents
// memory being allocated for each time an element is added. Which is nice.

template<typename T> bool container<T>::m_alloc(const int size)
{
	int temp_size = array_iterator + size;
	array_size = (int)(temp_size + temp_size / 4);
	
	T* n_mem_array = new T[array_size];
	
	for (int i = 0; i < array_iterator; i++)
	{
		n_mem_array[i] = object[i];
	}
	
	delete[] object;
	object = n_mem_array;
	
	std::cout << "MEMORY ALLOCATION: " << array_size << "\n";
	
	return true;
}

// removal_logic() handles the memory assignment logic of "remove" calls. It basically removes
// one or more elements and drop the gap that was created by the removal.

template<typename T> bool container<T>::removal_logic(const int remove_start, const int remove_end)
{
	if(remove_start <= remove_end && remove_end < array_iterator && remove_start >= 0)
	{
		T* n_mem_array = new T[array_size];
		int remove_range = (remove_end - remove_start) + 1;
		bool object_found = false;
		for(int i = 0; i < array_iterator - remove_range; i++)
		{
			if(i < remove_start || i > remove_end) 
			{
				if(object_found == false)
				{
					n_mem_array[i] = object[i];
				}
				else
				{
					n_mem_array[i] = object[i+remove_range];
				}
			}
			else
			{
				n_mem_array[i] = object[i+remove_range];
				object_found = true;
			}
		}
		delete[] object;
		object = n_mem_array;
		array_iterator = array_iterator - remove_range;
		std::cout << "OBJECTSSSSSS REMOVED!!!\n";
	}
	else
	{
		return false;
	}
	return true;
}

/* ****************************************************************************************************************************
 * 											DEFINITION OF THE PUBLIC FUNCTIONS
 * ****************************************************************************************************************************/

// operator= overload to copy other Containers. I wrote this with the "rule of three" in mind. I'm not sure 
// it does any good, though. It is likely that the compiler would do a better job.

template<typename T> container<T>& container<T>::operator= (const container<T>& copy)  noexcept
{
	array_size = copy.array_size;
	array_iterator = copy.array_iterator;
	T* n_mem_array = new T[array_size];
	for (int i = 0; i < array_size; i++)
	{
		n_mem_array[i] = copy.object[i];
	}
	delete[] object;
	object = n_mem_array;
	
	std::cout << "ASSIGNMENT OPERATOR!\n";
	return *this;
}

// operator[] overload to let users handle the Container as an array. I tried to limit the access to 
// initialized-only indexes. 

template<typename T> T& container<T>::operator[] (int object_location)
{
	if(object_location < array_iterator)
	{
		return object[object_location];	
	}
	else
	{
		throw out_of_range_exception();
	}
}


//Const version for const correctness
template<typename T> T container<T>::operator[] (int object_location) const
{
	if(object_location < array_iterator)
	{
		return object[object_location];	
	}
	else
	{
		throw out_of_range_exception();
	}
}

// default constructor.

template<typename T> container<T>::container()
{
	array_size = 0;
	array_iterator = 0;
	object = nullptr;
	
	std::cout << "DEFAULT CONSTRUCTOR\n";
}

// constructor with size initializer.

template<typename T> container<T>::container(const int size)
{
	array_size = size;
	array_iterator = 0;
	object = new T[array_size];
	std::cout << "REGULAR CONSTRUCTOR!\n";
}

// constructor with size and value initializers.

template<typename T> container<T>::container(const int size, const T& value)
{
	array_size = size;
	array_iterator = 0;
	object = new T[array_size];
	for (int i = 0; i < array_size; i++)
	{
		object[i] = value;
		array_iterator++;
	}
	
	std::cout << "VALUE CONSTRUCTOR!\n";  
}

template<typename T> container<T>::container(const T* start_address, const T* end_address)
{
	
	unsigned int size = (end_address-start_address)+1;
	std::cout << size;
	array_size = size;
	array_iterator = 0;
	object = new T[array_size];
	for (int i = 0; i < array_size; i++)
	{
		object[i] = start_address[i];
		array_iterator++;
	}
	
	std::cout << "\nADDRESS CONSTRUCTOR!\n";  
}

// copy constructor. Note that I didn't use move semantics due to lack of knowledge and the will
// to not use any libraries. This constructor, as you should expect, is very similar to the operator=
// overload. I had the "rule of three" in mind when I wrote this function. The compiler could
// do a better job, though.

template<typename T> container<T>::container(const container<T>& copy) noexcept
{
	array_size = copy.array_size;
	array_iterator = copy.array_iterator;
	object = new T[array_size];
	for (int i = 0; i < array_size; i++)
	{
		object[i] = copy.object[i];
	}
	
	std::cout << "COPY CONSTRUCTOR!\n";
}

// destructor.

template<typename T> container<T>::~container()
{
	delete[] object;
	std::cout << "DESTRUCTOR!\n";
}

// add() behaves like the vector's push_back() function. It adds an element to the end of the array.
// If the array is too short, than it allocates more memory by calling m_alloc() (see above).

template<typename T> bool container<T>::add(const T& in_object)
{
	if (array_iterator < array_size)
	{
		object[array_iterator] = in_object;
		array_iterator++;
	}
	else
	{
		m_alloc(1);
		object[array_iterator] = in_object;
		array_iterator++;
	}
	
	return true;
}

// this version of add() adds the whole content of another Container to the end of the array. It can be 
// used with the function range() like this container.add(container2.range(5, 10); (see below). If there is not 
// enough space in the array, more memory
// is allocated. 

template<typename T> bool container<T>::add(const container<T>& in_container)
{
	if (in_container.object != nullptr)
	{
		if (object == nullptr)
		{
			array_size = in_container.array_iterator;
			array_iterator = 0;
			object = new T[array_size];
		}
	
		if (array_size - array_iterator >= in_container.array_iterator)
		{
			for (int i = 0; i < in_container.array_iterator; i++)
			{
				object[array_iterator] = in_container.object[i];
				array_iterator++;
			}
		}
		else
		{
			m_alloc(in_container.array_iterator);
			for (int i = 0; i < in_container.array_iterator; i++)
			{
				object[array_iterator] = in_container.object[i];
				array_iterator++;
			}
		}
	}
	else 
	{
		/* 
		 * this return is VERY important! Ignore this comment, by the way. 
		 */
		std::cout << "ARGUMENT NOT INITIALIZED!!!!!!!!!\n";
		return false;
	}
	
	return true;
}

// add() returns the address of one element. This function may be redundant considenring
// the operator[] overload basicaly does the same thing.

template<typename T> T* container<T>::addr(const int object_location)
{
	if (object_location < array_iterator && object != nullptr)
	{
		return &object[object_location];
	}
	else 
	{
		return nullptr;
	}
}

// Index of the first element. Interface porposes. 

template<typename T> int container<T>::first_element_position() const
{
	return 0;
}

template<typename T> T* container<T>::begin()
{
	return &object[0];
}

// current ammount of memory allocated.

template<typename T> int container<T>::capacity() const
{
	return array_size;
}

// deletes the content of the Container.

template<typename T> bool container<T>::clear()
{
	if (object != nullptr)
	{
		delete[] object;
		object = new T[array_size];
		array_iterator = 0;
	}
	else
	{
		return false;
	}
	
	return true;
}

// checks if there is any element currently stored in the Container. 

template<typename T> bool container<T>::empty() const
{
	if (array_iterator > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// index of the last element.

template<typename T> int container<T>::last_element_position() const
{
	return array_iterator - 1;
}

template<typename T> T* container<T>::end()
{
	//if(array_iterator == 0)
	//{
	//	return &object[array_size];
	//}
	//else
	//{
		return &object[array_iterator];
	//}
	
}

// get() returns a copy of an element.

template<typename T> T container<T>::get(const int object_location) const
{
	if (object_location < array_iterator &&  object != nullptr)
	{
		return object[object_location];
	}
	else 
	{
		throw out_of_range_exception();
	}
}

// insert_single() inserts an element anywhere in the array, pushing the other elements forward.

template<typename T> bool container<T>::insert_single(const int object_location, const T& in_object)
{
	if(object_location < array_iterator)
	{
		if(array_size <= array_iterator)
		{
			array_size++;
		}
		
		T* n_mem_array = new T[array_size];
		bool insert_location_reached = false;
		for (int i = 0; i <= array_iterator; i++)
		{
			if(i < object_location)
			{
				n_mem_array[i] = object[i];
			}
			else
			{
				if(insert_location_reached == false)
				{
					n_mem_array[i] = in_object;
					insert_location_reached = true;
				}
				else
				{
					n_mem_array[i] = object[i-1];
				}
			}
		}
		delete[] object;
		object = n_mem_array;
		array_iterator++;			
	}
	else
	{
		return false;
	}
	return true;
}

// insert_range() inserts a whole Container anywhere in the array, pushing the other elements forward.

template<typename T> bool container<T>::insert_range(const int object_location, const container<T>& in_container)
{
	if(object_location < array_iterator)
	{
		if(array_size <= array_iterator + in_container.array_iterator)
		{
			array_size = array_size + in_container.array_iterator;
		}
		
		T* n_mem_array = new T[array_size];
		bool insert_location_reached = false;
		for (int i = 0; i <= array_iterator; i++)
		{
			if(i < object_location)
			{
				n_mem_array[i] = object[i];
			}
			else
			{
				if(insert_location_reached == false)
				{
					for(int k = 0; k < in_container.array_iterator; k++)
					{
						n_mem_array[i+k] = in_container.object[k];
					}
					insert_location_reached = true;
				}
				else
				{
					n_mem_array[(i+in_container.array_iterator)-1] = object[i-1];
				}
			}
		}
		delete[] object;
		object = n_mem_array;
		array_iterator = array_iterator + in_container.array_iterator;			
	}
	else
	{
		return false;
	}
	return true;
}

// pop() deletes the value of the last index of the array. Actually, what it really does is make the last 
// index available for overriding. The "popped" value will still be in memory until something 
// else is added to the Container.

template<typename T> bool container<T>::pop()
{
	if(array_iterator > 0)
	{
		array_iterator--;
	}
	else{
		return false;
	}
	
	return true;
}

// range() returns a new Container filtered by a any range. If you want the last 10 elements of a Container,
// you do this: Container<T> cont2 = cont1.range(cont1.end() - 10, cont1.end()); 

template<typename T> container<T> container<T>::range(const int start, const int end) const
{
	container<T> filtered_container;
	if (object != nullptr)
	{
		if (start < end && end < array_iterator)
		{
			filtered_container.array_size =  (end - start) + 1;
			filtered_container.array_iterator = 0;
			filtered_container.object = new T[filtered_container.array_size];
			for (int i = 0; i < filtered_container.array_size; i++)
			{
				filtered_container.object[i] = object[start + i];
				filtered_container.array_iterator++;
			}
		}
	}
	
	return filtered_container;
}

// remove_single() removes an element from anywhere in the array, then drops the gap
// created by the removal.

template<typename T> bool container<T>::remove_single(const int object_location)
{
	return removal_logic(object_location, object_location);
}

// remove_range() removes a sequence of elements from anywhere in the array, then drops
// the gap created by the removal.

template<typename T> bool container<T>::remove_range(const int start_location, const int end_location)
{
	return removal_logic(start_location, end_location);
}

// replace() replaces any value in array. Could be redundant considering the operator[] overload.

template<typename T> bool container<T>::replace(const int object_location, const T& in_object)
{
	if(object_location < array_iterator)
	{
		object[object_location] = in_object;
	}
	else
	{
		return false;
	}
	
	return true;
}

// shrink() gets rid of all the excessive memory in the array. If you have 10 elements stored in a Container
// that has 30 slots, afther shrinking you'll have 10 elements and 10 memory slots.

template<typename T> bool container<T>::shrink()
{
	if (object != nullptr && array_size > array_iterator)
	{
		if (array_iterator > 0)
		{
			array_size = array_iterator;
			T* n_mem_array = new T[array_size];
			
			for (int i = 0; i < array_iterator; i++)
			{
				n_mem_array[i] = object[i];
			}
			
			delete[] object;
			object = n_mem_array;
		}
		else
		{
			delete[] object;
			object = nullptr;
			array_size = 0;
		}
		
	}
	else
	{
		return false;
	}
	
	return true;
}

// returns the current amount of elements stored in the Container.

template<typename T> int container<T>::size() const
{
	return array_iterator;
}

}

#endif // CONTAINER_H


/*
template<typename T> void Container<T>::object_destructor(const int start, const int end)
{
	for(int i = start; i <= end; i++)
	{
		object[i].~T();
		std::cout << "INSIDE OBJECT_DESTRUCTOR\n";
	}
}*/
