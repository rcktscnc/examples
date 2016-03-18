#include <memory>
#include <iostream>

void unique_ptr_example() {
	int n;
	std::cout << "Array size: ";
	std::cin >> n;
	std::unique_ptr<int[]> array(new int[n]);
	
	for(int i = 0; i < n; i++) {
		array[i] = i*10;
		std::cout << array[i] << "\n";
	}
}

