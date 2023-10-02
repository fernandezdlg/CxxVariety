#include <cassert>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;


struct vec{
    size_t capacity; // max number of elements that can be stored
    size_t size; // number of elements currently stored
    double* data; // pointer to the first element

    // Default Constructor
    vec(){
        this->capacity = 10;
        this->size = 0;
        this->data = new double[capacity];
    }
    
    // Destructor
    ~vec(){
        delete[] data;
    }

    // With capacity
    vec(size_t capacity){
        this->capacity = capacity;
        this->size = 0;
        this->data = new double[capacity];
    }

    // Copy constructor
    vec(const vec& other){
        this->capacity = other.capacity;
        this->size = other.size;
        this->data = new double[capacity];
        for (size_t i = 0; i < size; i++){
            this->data[i] = other.data[i];
        }
    }

    // Methods
    void pushback(double x) {
        if (size == capacity){ // if array is full
            capacity *= 2;
            double* new_data = new double[capacity];
            for (size_t i = 0; i < size; i++){
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
        }
        data[size] = x;
        size++;
    }

    // For cout: print the vector
    friend std::ostream& operator<<(std::ostream& os, const vec& v){
        os << "[";
        for (size_t i = 0; i < v.size; i++){
            os << v.data[i];
            if (i < v.size - 1){
                os << ", ";
            }
        }
        os << "]";
        return os;
    }
};


int main(){
    
    vec v;
    v.pushback(1);
    v.pushback(2);
    cout << v << endl;

    v.pushback(3);
    cout << v << endl;

    // delete v
    v.~vec();
    
    // This will print trivial values and throw errors:
//  cout << v << endl; 
    // i.e.:
    // [0, 0, 0]
    // main(1853,0x1de4c1300) malloc: Double free of object 0x15ae05e00
    // main(1853,0x1de4c1300) malloc: *** set a breakpoint in malloc_error_break to debug
    // [1]    1853 abort      ./main

    return 0;
}