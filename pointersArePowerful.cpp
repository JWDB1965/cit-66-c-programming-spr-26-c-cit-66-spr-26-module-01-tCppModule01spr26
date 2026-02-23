#include <iostream>
using namespace std;

int main() {
   
    int a = 10;
    int* p = &a; 

    cout << "Address stored in p: " << p << endl;
    cout << "Value of a (via p): " << *p << endl;

    *p = 20; 
    cout << "New value of a: " << a << endl << endl;

   
    int arr[5] = {10, 20, 30, 40, 50};
    int* ptr = arr; 
    cout << "Array element addresses and values:" << endl;
    for (int i = 0; i < 5; i++) {
       
        cout << "Address: " << (ptr + i) << " | Value: " << *(ptr + i) << endl;
    }

    return 0;
}
