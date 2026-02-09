#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string userInput;
      
    std::cout << "Enter a string to reverse: ";
    std::getline(std::cin, userInput);
    
    std::string reversedStr = "";
    
    // Loop backwards from the last character to the first
    for (int i = userInput.length() - 1; i >= 0; i--) {
        reversedStr += userInput[i];
    }
    
    std::cout << "Reversed string: " << reversedStr << std::endl;
    
    return 0;
}
