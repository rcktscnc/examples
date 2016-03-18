#include <iostream>
#include "move_semantics_example.h" // gms::forward()

template<typename T>
struct remove_array {
    typedef T type;
    static bool is_array();
};
template<typename T> bool remove_array<T>::is_array() {
    return false;
}

template<typename T>
struct remove_array<T[]> {
    typedef T type;
    static bool is_array();
};
template<typename T> bool remove_array<T[]>::is_array() {
    return true;
}

// my own shared_pointer. 
template<typename T>
class shared_pointer {
private:
    
    size_t* _refs;
    typename remove_array<T>::type* _obj;
    
    void _copy(const shared_pointer& origin) {
        _obj = origin._obj;
        _refs = origin._refs;
        ++(*_refs);
    }
    
public:
    
    shared_pointer() = delete;
    
    shared_pointer(typename remove_array<T>::type* obj) {
        _obj = obj;
        _refs = new size_t(0);
    }
    
    shared_pointer(const shared_pointer& origin) {
        _copy(origin);
    }
    
    shared_pointer& operator=(const shared_pointer& origin) {
        _copy(origin);
    }
    
    ~shared_pointer() {
        if (*_refs  == 0) {
            if (remove_array<T>::is_array()) {
                delete[] _obj;
            } else {
                delete _obj; 
            }
            delete _refs;
        } else {
            --(*_refs);
        }
    }
    
    T& operator*() {
        return *_obj;
    }
    
    T& operator[](size_t index) {
        return *(_obj + index);
    }
    
    T* operator->() {
        return _obj;
    }
};

// my own make_shared. It is incomplete though. It doesn't
// support arrays or custom deleters. 100% copied from a book
template<typename T, typename... Args>
shared_pointer<T> make_shared(Args&&... args) {
    return shared_pointer<T>(new T(gms::forward<Args>(args)...));
}