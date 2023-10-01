#include <cassert>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

/// @brief Prints the binary representation of an integer
/// @param x is the integer to be printed
void intToBits(int x){
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--){
        cout << ((x >> i) & 1);
    }   
}

/// @brief Prints the binary representation of a float
/// @param x is the float to be printed
void floatToBits(float x){
    assert(sizeof(float) == sizeof(int));
    // ! DO NOT cast float to int, but instead read directly from memory => use pointers
    int* p = (int*) &x;
    intToBits(*p);
}

int main(){
    
    // Check number representation of int:
    int x = 5;
    cout << "Number representation of int " << x << " is:" << endl;
    intToBits(x);
    cout << "\n\n";

    // Check number representation of float:
    float y = 5.0;
    cout << "Number representation of float " << y << " is:" << endl;
    floatToBits(y);
    cout << "\n\n";

    return 0;
}