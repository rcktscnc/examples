#ifndef _MOVE_SEMANTICS_EXAMPLE
#define _MOVE_SEMANTICS_EXAMPLE

#include <iostream>
#include <utility>
#include "type_traits_example.h" // remove_reference<T>

namespace gms {
    template<typename T>
    typename remove_reference<T>::type&& move(T&& arg) {
        return ((typename remove_reference<T>::type&&)arg);
    }
    
    template<typename T>
    T&& forward(typename remove_reference<T>::type& arg) {
        return ((T&&)arg);
    }

    template<typename T>
    void swap(T& a, T& b) {
        T c(move(a));
        a = move(b);
        b = move(c);
        std::cout << "SWAP!" << std::endl;
    }
} // namespace

class dynamic_array_int {
private:

    unsigned int _size;
    int* _ptr;
    
    void swap(dynamic_array_int& copy) { // class-specific swap function
        gms::swap(_size, copy._size); // using my own swap instead of std::swap
        gms::swap(_ptr, copy._ptr);
    }
    
public:

    dynamic_array_int() { // default constructor
        _size = 0;
        _ptr = nullptr;
        std::cout << "DEFAULT Constructor!" << std::endl;
    }
    
    dynamic_array_int(unsigned int size) { // unsigned int constructor
        _size = size;
        _ptr = new int[size];
        std::cout << "REGULAR Constructor!" << std::endl;
    }
    
    dynamic_array_int(const dynamic_array_int& copy) // copy constructor 
        : dynamic_array_int(copy._size) { // initialize with uint constructor
        for (unsigned int i = 0; i < copy._size; ++i) { 
            *(_ptr + i) = *(copy._ptr + i);
        }
        std::cout << "COPY Constructor!" << std::endl;
    }
    
    dynamic_array_int(dynamic_array_int&& copy) noexcept // move constructor
        : dynamic_array_int() { // initialize with default constructor
        dynamic_array_int::swap(copy); // using class-specific swap
        std::cout << "MOVE Constructor!" << std::endl;
    }
    
    ~dynamic_array_int() { // destructor
        delete[] _ptr;
    }
    
    // When using copy-and-swap idiom, there's no need to explicitly overload
    // a move assignment operator, because copy-and-swap idiom uses both
    // copy constructor and move constructor depending on the value passed to
    // it. If the value passed is an lvalue, copy constructor will be called;
    // if the value is an rvalue, move constructor will be called. Thus, no 
    // unecessary copies are made during copy-and-swap idiom.
    // copy-and-swap idiom:
    dynamic_array_int& operator=(dynamic_array_int copy) noexcept {
        dynamic_array_int::swap(copy); // using class-specific swap
        std::cout << "copy-and-swap idiom" << std::endl;
        return *this;
    }
    
    int& operator[](unsigned int index) { 
        return *(_ptr + index);
    }
    
    int operator[](unsigned int index) const {
        return *(_ptr + index);
    }
};

#endif // _MOVE_SEMANTICS_EXAMPLE