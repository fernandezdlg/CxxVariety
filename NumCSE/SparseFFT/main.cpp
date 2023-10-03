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


template<class T> struct sparse_vec {
    double tol = 1e-6;
    vector<duplet<T>> duplets;
    int len;

    /// @brief Constructor
    /// @param length is the length of the sparse vector
    sparse_vec(int length) : len(length) {}

    /// @brief Copy constructor
    /// @param other is the sparse vector to copy
    sparse_vec(const sparse_vec<T> &other) : tol(other.tol), len(other.len) {
        duplets = other.duplets;
    }

    /// @brief Appends duplet to sparse_vec
    /// @param index is the index of the value to append
    /// @param value is the value to append
    void append(int index, T value){
        duplets.push_back(duplet<T>(index, value));
        len++;
    }
    
    /// @brief Cleans up the sparse vector
    void cleanup(){
        
        std::sort(duplets.begin(), duplets.end(), [](duplet<T> a, duplet<T> b){return a.index < b.index;});

        for (int i = 0; i < duplets.size()-1; i++){
            while (duplets[i].index == duplets[i+1].index){
                duplets[i].value += duplets[i+1].value;
                duplets.erase(duplets.begin() + i);
                len--;
            }
        }
        for (int i = 0; i < duplets.size(); i++){ // can we use duplet : duplets? because the index i is volatile
            while (abs(duplets[i].value) < tol){
                duplets.erase(duplets.begin() + i);
                len--;
            }
        }
        for (int i = 0; i < duplets.size(); i++){ // can we use duplet : duplets? because the index i is volatile
            while (duplets[i].index > len){
                duplets.erase(duplets.begin() + i);
                len--;
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
        return _get_val(index, 0, duplets.size()); // do it smarter!
    }

    /// @brief helper function for get_val, using binary search
    /// @param n is the index
    /// @param n1 is the lower bound
    /// @param n2 is the upper bound
    /// @return the value of the duplet with index n
    T _get_val(int n, int n1, int n2) const {
        if (n1 > n2){
            return 0;
        }
        int mid = (n1 + n2) / 2;
        if (duplets[mid].index == n){
            return duplets[mid].value;
        }
        if (duplets[mid].index < n){
            return _get_val(n, mid+1, n2);
        }
        if (duplets[mid].index > n){
            return _get_val(n, n1, mid-1);
        }
    }
};


int main(){

	sparse_vec<complex<double>> x(5);
    cout << "X length " << x.len << endl;
    x.append(0,complex<double>(8.2,0));
    cout << "X length " << x.len << endl;
	x.append(1,complex<double>(1,-2));
    cout << "X length " << x.len << endl;
	x.append(3,complex<double>(-3,4.66));
    cout << "X length " << x.len << endl;
	x.append(4,complex<double>(0,4));
    cout << "X length " << x.len << endl;


    for (int i = 0; i < x.len; i++){
        cout << x.get_val(i) << endl;
    }
    cout << "X length " << x.len << endl;
	x.cleanup();
    return 0;
}