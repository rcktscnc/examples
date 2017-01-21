template<typename T>
T sum(T arg) {
	return arg;
}
template<typename T, typename... Args>
T sum(T first, Args... args) {
	return first + sum<T>(args...);
}

//////////////////////////////////////
/*
template<typename... Args>
void call_each(Args...) { }

template<typename Func, typename... Args>
void apply(Func func, Args... args) {
	call_each( func(args)... );
}
*/
//////////////////////////////////////

template<typename Func, typename T>
void apply(Func func, T arg) {
	func(arg);
}
template<typename Func, typename T, typename... Args>
void apply(Func func, T first, Args... args) {
	func(first);
	apply(func, args...);
}