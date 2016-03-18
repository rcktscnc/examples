template<typename T, typename Function>
T lamb(T value, Function func) { // receives a lambda or a function pointer
	return func(value);
}