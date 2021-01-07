#include <iostream>
#include "hello.h"

void __declspec(dllexport) PrintHello()
{
	std::cout << "Hello World" << std::endl;
}



