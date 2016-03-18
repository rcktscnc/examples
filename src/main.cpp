// Baleia "Caxalote"

#include <iostream>
#include <string>

#include "unique_ptr_example.h"
#include "variadic_templates_example.h"
#include "lambda_example.h"
#include "pure_virtual_example.h"
#include "dynamic_casting_example.h"
#include "reference_to_array_example.h"
#include "move_semantics_example.h"
#include "map_example.h"
#include "shared_ptr_example.h"
#include "init_list_example.h"

int main(int argc, char **argv) {
    
    
    
    /*
    dynamic_array_int asd(10);
    
    for (int i = 0; i < 10; ++i) {
        asd[i] = 10;
        std::cout << asd[i] << std::endl;
    }
    
    dynamic_array_int bsd(asd);
    
    for (int i = 0; i < 10; ++i) {
        
        std::cout << asd[i] << std::endl;
    }
    */
    //dummy d(init_list<int>(1, 4, 17, 2));
    
    /*
    dynamic_array_int a(10);
    dynamic_array_int b(10);
    for (int i = 0; i < 10; ++i) {
        a[i] = i;
        b[i] = i*10;
    }
    */
    //some(10);
    /*
    int num = 10;
    shared_pointer<dynamic_array_int> ptr(make_shared<dynamic_array_int>(10)); 
    
    for (int i = 0; i < 10; ++i) {
        (*ptr)[i] = i;
    }
    
    for (int i = 0; i < 10; ++i) {
        std::cout << (*ptr)[i] << std::endl;
    }
    */
    
    //dynamic_array_int asd(a);
    
    /*
    ptr[0] = a;
    ptr[1] = b;
    
    for (int i = 0; i < 10; ++i) {
        std::cout << ptr[0][i] << std::endl;
    }
    
    for (int i = 0; i < 10; ++i) {
        std::cout << ptr[1][i] << std::endl;
    }*/
    
    //ptr[1] = 10;
    
    //simple_pointer<int[]> sptr;
    //sptr.signal();
    
    
    //ctrl();
    
	/*
    dynamic_array_int arr(10);
    for (int i = 0; i < 10; ++i) {
        arr[i] = i;
    }
    
    std::cout << "---------------------" << std::endl;
    
    arr = dynamic_array_int(2);
    */
    
    //for (int i = 0; i < 10; ++i) {
    //    std::cout << arr[i] << std::endl;
    //}
    
    //dynamic();
	
	//int(*p)(int) = [](int n){ std::cout << n + 10 << "\n"; return 0; };
	//auto lamb = [](std::string s){ std::cout << s << "\n"; return 0; };
	//apply(p, 1, 2, 3);
	
	//apply(lamb, "Donald ", "Trump ", " Funny");
	//std::cout << sum(1, 2, 3, 4, 5, 6);
	//std::cout << lamb<std::string, std::string(std::string)>(std::string("nostradamus"), [](std::string v){return v+v;});
	
	//base b(3);
	//derived d(5);
	//std::cout << pve_func(d);
	
	return 0;
}
