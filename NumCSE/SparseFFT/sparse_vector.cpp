#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <complex>

using std::cout;
using std::endl;
using std::complex;
using std::vector;

const complex<double> I(0,1);

/// @brief Struct for a duplet (index, value)
/// @tparam T is the type of the value
template<class T> struct duplet {
    int index;
    T value;    

    /// @brief Constructor
    /// @param index is the index of the duplet
    /// @param value is the value at the index
    duplet(int index, T value) : index(index), value(value) {}
};

/// @brief Struct for a sparse vector
/// @tparam T is the type of the values
template<class T> struct sparse_vec {
    double tol = 1e-6;
    vector<duplet<T>> duplets;
    int len = 0; // length of the sparse vector, is NOT modifyable after construction.

    /// @brief Constructor
    /// @param len is the length of the sparse vector
    sparse_vec(int len) : len(len) {}

    /// @brief Copy constructor
    /// @param other is the sparse vector to copy
    sparse_vec(const sparse_vec<T> &other) : tol(other.tol), len(other.len), duplets(other.duplets) {}

    /// @brief Appends duplet to sparse_vec
    /// @param index is the index of the value to append
    /// @param value is the value to append
    void append(int index, T value){
        if (abs(value) < tol){
            return;
        }
        duplets.push_back(duplet<T>(index, value));
    }
    

    /// @brief Cleans up the sparse vector
    void cleanup(){
        
        // sort duplets by index
        std::sort(duplets.begin(), duplets.end(), [](duplet<T> a, duplet<T> b){return a.index < b.index;});

        // join duplicates 
        int duplets_size = duplets.size();        
        for (int i = 0; i < duplets_size-1; i++){
            if (duplets[i].index == duplets[i+1].index){
                duplets[i].value += duplets[i+1].value;
                duplets.erase(duplets.begin()+i+1);
                duplets_size--;
                i--;
            }
        }
        // delete duplets with abs(value) < tol
        // I think this is not needed because in append there is a filter already.
        duplets_size = duplets.size();      
        for (int i = 0; i < duplets_size; i++){ // can we use duplet : duplets? because the index i is volatile
            if (abs(duplets[i].value) < tol){
                duplets.erase(duplets.begin()+i);
                duplets_size--;
                i--;
            }
        }
        // delete duplets with index >= len
        duplets_size = duplets.size();      
        for (int i = 0; i < duplets_size; i++){ // can we use duplet : duplets? because the index i is volatile
            if (duplets[i].index > len - 1){
                duplets.erase(duplets.begin()+i);
                duplets_size--;
                i--;
            }   
        }

    }


    /// @brief sparse vector assumed to be already ``cleaned up''
    /// @param index is the index
    /// @return the value of the duplet with index index
    T get_val(int index) const {
        if (duplets.empty()){
            return 0;
        }
        return _get_val(index, 0, len-1); // do it efficiently with binary search
    }

    /// @brief helper function for get_val, using binary search
    /// @param n is the index to search for
    /// @param n1 is the lower bound (inclusive)
    /// @param n2 is the upper bound (inclusive)
    /// @return the value of the duplet with index n
    T _get_val(int n, int n1, int n2) const {
        if (n1 > n2){
            return 0;
        }
        int mid = (n1 + n2) / 2; // floored
        if (duplets[mid].index == n){
            return duplets[mid].value;
        }
        if (duplets[mid].index < n){
            return _get_val(n, mid+1, n2);
        }
        if (duplets[mid].index > n){
            return _get_val(n, n1, mid-1);
        } else {
            std::cout << "Something went wrong in _get_val" << std::endl;
            return 0;
        }
    }
};


int main(){

	sparse_vec<complex<double>> x(5);
    x.append(3,complex<double>(8.2,0));
	x.append(1,complex<double>(1,-2));
	x.append(4,complex<double>(-3,4.66)); // this should be summed
	x.append(4,complex<double>(0,4));  // this should be summed
    x.append(5,complex<double>(2,3)); // this should be deleted

	x.cleanup();

    cout << "Printing values at all indices in the sparse vector: \n";
    for (int i = 0; i < x.len; i++){
        cout << x.get_val(i) << endl;
    }
    cout << "\n Actual content in x.duplets: \n";
    for (auto d : x.duplets){
        cout << d.index << " " << d.value << endl;
    }

    return 0;
}
