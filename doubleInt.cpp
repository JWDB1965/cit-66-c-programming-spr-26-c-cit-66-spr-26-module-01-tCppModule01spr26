#include <iostream>

int main() {
    int number;

    std::cout << "Enter an integer: ";

    std::cin >> number;

    
    int doubled_number = number * 2;

    
    std::cout << "The double of " << number << " is " << doubled_number << "." << std::endl;

    return 0;
}
