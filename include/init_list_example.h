#include <iostream>
#include <vector>
#include <utility>

// I have no idea how std::initializer_list<> really works, but I implemented
// this class to work in a vaguely "similar" way. (It's not similar at all)
// operator {} is not overloadable, apparently. so I have no clue how to
// create something similar to initializer_list<>.
template<typename T>
class init_list {
private:

    std::vector<T> _list;
    
    template<typename Arg>
    void populate(Arg&& arg) {
        _list.push_back(std::forward<Arg>(arg));
    }
    
    template<typename First, typename... Args>
    void populate(First&& first, Args&&... args) {
        _list.push_back(std::forward<First>(first));
        populate(args...);
    }
    
    init_list();
    
    init_list(const init_list<T>&);
    
    init_list<T>& operator=(init_list<T>);
    
public:

    init_list(init_list<T>&& src) : _list(std::move(src._list)) { 
        std::cout << "move construction" << std::endl;
    }
    
    template<typename... Args>
    init_list(Args&&... args) {
        std::cout << "ARGS construction" << std::endl;
        populate(std::forward<Args>(args)...);
    }
    
    T* begin() {
        return &_list[0];
    }
    
    T* end() {
        return &_list[_list.size()];
    }
    
    std::vector<T>&& move_vector() {
        return std::move(_list);
    }
    
};

class dummy {
    public:
    std::vector<int> vec;
    dummy(init_list<int> list) 
        : vec(list.move_vector()) {
        //for (auto& e : list) {
        //    vec.push_back(std::move(e));
        //}
    }
};