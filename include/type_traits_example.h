namespace gms { // my own version of remove_reference<>
    
    // remove_reference removes reference from both rvalues and lvalues.
    // It is used by gms::move()
    template<typename T>
    struct remove_reference {
        typedef T type;
    };

    template<typename T>
    struct remove_reference<T&> {
        typedef T type;
    };

    template<typename T>
    struct remove_reference<T&&> {
        typedef T type;
    };

} // namespace