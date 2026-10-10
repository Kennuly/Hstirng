#include <iostream>
#include "Hstring.h"
#include <string>

int main()
{
    Hstring str;
    std::cout << "str: " << str.c_str() << ", length: " << str.length() << std::endl;

    Hstring str1("Hello");
    std::cout << "str1: " << str1.c_str() << ", length: " << str1.length() << std::endl;

    Hstring str2(str1);
    std::cout << "str2: " << str2.c_str() << ", length: " << str2.length() << std::endl;
    
    str1 - "ll";
    std::cout << "str1 after removing 'll': " << str1.c_str() << ", length: " << str1.length() << std::endl;

    str1.replace(1, 2, "ey");
    std::cout << "str1 after replacing 'el' with 'ey': " << str1.c_str() << ", length: " << str1.length() << std::endl; 
    
    if(str1.find("h") != -1)
    {
        std::cout << "'h' found in str1" << std::endl;
    }
    else
    {
        std::cout << "'h' not found in str1" << std::endl;
    }
    str = str1;
    std::cout << "str: " << str.c_str() << ", length: " << str.length() << std::endl;
    str = 1234567;
    std::cout << "str: " << str.c_str() << ", length: " << str.length() << std::endl;

    return 0;
}