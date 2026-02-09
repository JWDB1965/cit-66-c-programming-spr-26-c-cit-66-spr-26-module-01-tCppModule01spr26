#include <iostream>

int main() {
    int n1, n2, n3;
    
    std::cout << "Enter three integers separated by spaces: ";
    std::cin >> n1 >> n2 >> n3;
    
    int largest;
    
    if (n1 >= n2) {
        if (n1 >= n3) {
            largest = n1;
        } else {
            largest = n3;
        }
    } else {
        if (n2 >= n3) {
            largest = n2;
        } else {
            largest = n3;
        }
    }
    
    std::cout << "The largest integer is: " << largest << std::endl;
    
    return 0;
}
