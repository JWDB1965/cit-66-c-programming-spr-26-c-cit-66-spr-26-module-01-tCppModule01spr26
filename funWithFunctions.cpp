#include <iostream>

// Function prototypes
int getAnIntFromTheUser();
void compareTwoInts(int a, int b);
int sumTwoInts(int a, int b);

int main() {
    std::cout << "--- Fun with Functions ---" << std::endl;
    
    int num1 = getAnIntFromTheUser();
    int num2 = getAnIntFromTheUser();
    
    compareTwoInts(num1, num2);
    
    int sum = sumTwoInts(num1, num2);
    std::cout << "The sum of " << num1 << " and " << num2 << " is: " << sum << std::endl;
    
    return 0;
}

int getAnIntFromTheUser() {
    int input;
    std::cout << "Enter an integer: ";
    std::cin >> input;
    return input;
}

void compareTwoInts(int a, int b) {
    if (a > b) {
        std::cout << a << " is greater than " << b << std::endl;
    } else if (b > a) {
        std::cout << b << " is greater than " << a << std::endl;
    } else {
        std::cout << "Both numbers are equal." << std::endl;
    }
}

int sumTwoInts(int a, int b) {
    return a + b;
}
