#include <cassert>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

/// @brief Function to initialize a vector with 10e8 values, the first one is 1.0, and the rest are random values around [1e-8, 2e-8]
/// @tparam T is the type of the vector (e.g. vector<float>)
/// @param v is the vector to be initialized
template <class T> void initOneFirst(T &v) {
    // initialize v as 10e8 random values around [1e-8, 2e-8]
    v.push_back(1.0);
    for (int i = 0; i < 1e8 - 1; i++){
        v.push_back(1e-8 + (float)rand()/RAND_MAX * 1e-8);
    }
}

/// @brief Function to initialize a vector with 10e8 values, the last one is 1.0, and the rest are random values around [1e-8, 2e-8]
/// @tparam T is the type of the vector (e.g. vector<float>)
/// @param v is the vector to be initialized
template <class T> void initOneLast(T &v) {
    // initialize v as 10e8 random values around [1e-8, 2e-8]
    for (int i = 0; i < 1e8 - 1; i++){
        v.push_back(1e-8 + (float)rand()/RAND_MAX * 1e-8);
    }
    v.push_back(1.0);
}

/// @brief Function to compute the sum of a vector of values by ordering from smallest to largest first.
/// @tparam T is the typename (e.g. float)
/// @param v is the vector to be summed
/// @return The sum of the vector
template <typename T> T semiaccurateSum(vector<T> &v) {
    // sum first the smallest values
    // => order ascendingly
    std::sort(v.begin(), v.end());
    // Then sum
    T sum = 0.0;
    for (T x : v) sum += x;
    return sum;
}


/// @brief Function to compute the sum of a vector of values accurately (Kahan algorithm).
/// @tparam T is the typename (e.g. float)
/// @param v is the vector to be summed
/// @return The sum of the vector
template <typename T> T accurateSum(vector<T> &v) {
    T sum = 0.0;
    T errAccumulator = 0.0;
    for (T x : v) {
        errAccumulator += x; // This variable tracks the sum of rounding error
        T tmp = errAccumulator + sum; // The discarded rounding error is re-added to the sum at next iteration
        errAccumulator -= (tmp - sum);
        sum = tmp;
    }
    return sum;
}


int main(){

    vector<float> vi;
    vector<float> vf;

    // initialize vi and vf
    initOneFirst(vi);
    initOneLast(vf);

    // compute sum of vi and vf
    float sumi = 0.0;
    float sumf = 0.0;
    for (float x : vi) sumi += x;
    for (float x : vf) sumf += x;

    cout << "The sum for vi is " << sumi << endl; // This should be 1.0 due to rounding error in the float format
    cout << "The sum for vf is " << sumf << endl; // This should be in (1.1, 1.2)

    // compute the semiaccurate sum of vi and vf
    float saccSumi = semiaccurateSum(vi);
    float saccSumf = semiaccurateSum(vf);

    cout << "The semiaccurate sum for vi is " << saccSumi << endl; // This should be in (1.1, 1.2)
    cout << "The semiaccurate sum for vf is " << saccSumf << endl; // This should be in (1.1, 1.2)
    
    // compute the accurate sum of vi and vf
    float accSumi = accurateSum(vi);
    float accSumf = accurateSum(vf);

    cout << "The accurate sum for vi is " << accSumi << endl; // This should be in (1.1, 1.2)
    cout << "The accurate sum for vf is " << accSumf << endl; // This should be in (1.1, 1.2)

    return 0;
}