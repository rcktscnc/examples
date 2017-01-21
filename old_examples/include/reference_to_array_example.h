int(&verbose(int(&(*func_ptr)(int(&array)[5]))[5], int(&array)[5]))[5]
{ return func_ptr(array); }

void verbose2() {
	int(&(*f_ptr)(int, int(&)[5]))[5] = [](int x, int(&ref)[5])->int(&)[5] 
	{ for(int i=0; i<5; i++) ref[i] = x; return ref; };
	
	int array[] = {0, 0, 0, 0, 0};
	
	int(&result_array)[5] = f_ptr(10, array);
	
	for(int i = 0; i < 5; i++) std::cout << array[i] << "\n";
	for(int i = 0; i < 5; i++) std::cout << result_array[i] << "\n";
}

int(&f(int(&a)[5]))[5] {
   return a;
}

int(&(*func_ptr)(int(&a)[5]))[5] = f;