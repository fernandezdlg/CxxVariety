#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <complex>

using std::complex;
using std::cout;
using std::endl;
using std::vector;

const complex<double> I(0, 1);
const double PI = 3.14159265359;


/// @brief Struct for a duplet (index, value)
/// @tparam T is the type of the value
template<class T>
struct duplet
{
    int index;
    T   value;

    /// @brief Constructor
    /// @param index is the index of the duplet
    /// @param value is the value at the index
    duplet(int index, T value)
    : index(index)
    , value(value)
    {
    }
};

/// @brief Struct for a sparse vector
/// @tparam T is the type of the values, if using FFT, T should be complex
template<class T>
struct sparse_vec
{
    double            tol = 1e-6;
    vector<duplet<T>> duplets;
    int len = 0;  // length of the sparse vector, is NOT modifyable after construction.

    /// @brief Constructor
    /// @param len is the length of the sparse vector
    sparse_vec(int len)
    : len(len)
    {
    }

    /// @brief Copy constructor
    /// @param other is the sparse vector to copy
    sparse_vec(const sparse_vec<T>& other)
    : tol(other.tol)
    , len(other.len)
    , duplets(other.duplets)
    {
    }

    /// @brief Appends duplet to sparse_vec
    /// @param index is the index of the value to append
    /// @param value is the value to append
    void append(int index, T value)
    {
        if (abs(value) < tol)
        {
            return;
        }
        duplets.push_back(duplet<T>(index, value));
    }


    /// @brief Cleans up the sparse vector
    void cleanup()
    {
        // sort duplets by index O(log n)
        std::sort(duplets.begin(),
                  duplets.end(),
                  [](duplet<T> a, duplet<T> b) { return a.index < b.index; });

        // join duplicates
        int duplets_size = duplets.size();
        for (int i = 0; i < duplets_size - 1; i++)
        {
            if (duplets[i].index == duplets[i + 1].index)
            {
                duplets[i].value += duplets[i + 1].value;
                duplets.erase(duplets.begin() + i + 1);
                duplets_size--;
                i--;
            }
        }
        // delete duplets with abs(value) < tol
        // I think this is not needed because in append there is a filter already.
        duplets_size = duplets.size();
        for (int i = 0; i < duplets_size; i++)
        {  // can we use duplet : duplets? because the index i is volatile
            if (abs(duplets[i].value) < tol)
            {
                duplets.erase(duplets.begin() + i);
                duplets_size--;
                i--;
            }
        }
        // delete duplets with index >= len
        duplets_size = duplets.size();
        for (int i = 0; i < duplets_size; i++)
        {  // can we use duplet : duplets? because the index i is volatile
            if (duplets[i].index > len - 1)
            {
                duplets.erase(duplets.begin() + i);
                duplets_size--;
                i--;
            }
        }
    }


    /// @brief sparse vector assumed to be already ``cleaned up''
    /// @param index is the index
    /// @return the value of the duplet with index index
    T get_val(int index) const
    {
        if (duplets.empty())
        {
            return 0;
        }
        return _get_val(index, 0, len - 1);  // do it efficiently with binary search = O(log n)
    }

    /// @brief helper function for get_val, using binary search
    /// @param n is the index to search for
    /// @param n1 is the lower bound (inclusive)
    /// @param n2 is the upper bound (inclusive)
    /// @return the value of the duplet with index n
    T _get_val(int n, int n1, int n2) const
    {
        if (n1 > n2)
        {
            return 0;
        }
        int mid = (n1 + n2) / 2;  // floored
        if (duplets[mid].index == n)
        {
            return duplets[mid].value;
        }
        if (duplets[mid].index < n)
        {
            return _get_val(n, mid + 1, n2);
        }
        if (duplets[mid].index > n)
        {
            return _get_val(n, n1, mid - 1);
        }
        else
        {
            std::cout << "Something went wrong in _get_val" << std::endl;
            return 0;
        }
    }

    /// @brief Fucntion to compute the componenwise product of the two sparse vectors
    /// @param a is a sparse vector
    /// @param b is a sparse vector
    /// @return The componentwise product of a and b as a sparse vector
    static sparse_vec cwise_mult(const sparse_vec& a, const sparse_vec& b)
    {
        sparse_vec out(std::max(a.len, b.len));

        // // This depends on the .get_val() complexity => O(n log n)
        // if (out.len == a.len)
        //     for (auto d : a.duplets){
        //         out.append(d.index, d.value * b.get_val(d.index));
        //     }
        // else
        //     for (auto d : b.duplets){
        //         out.append(d.index, d.value * a.get_val(d.index));
        //     }

        // This is better, assumes both sparse vectors are already cleaned up => O(n)
        int ia = 0, ib = 0;
        while (ia < a.duplets.size() && ib < b.duplets.size())
        {
            if (a.duplets[ia].index == b.duplets[ib].index)
            {
                out.append(a.duplets[ia].index, a.duplets[ia].value * b.duplets[ib].value);
                ia++;
                ib++;
            }
            else if (a.duplets[ia].index < b.duplets[ib].index)
            {
                ia++;
            }
            else
            {
                ib++;
            }
        }
        return out;
    }

    /// @brief Function to compute the convolution for two sparse vectors
    /// @param a is a sparse vector
    /// @param b is a sparse vector
    /// @return The convolution of a and b as a sparse vector
    static sparse_vec conv(const sparse_vec& a, const sparse_vec& b)
    {
        sparse_vec out(a.len + b.len - 1);
        // O(n^2)
        for (auto da : a.duplets)
        {
            for (auto db : b.duplets)
            {
                // This is each term in c(i) = sum_j f(j)g(i-j)
                out.append(da.index + db.index, da.value * db.value);
            }
        }
        // O(n log n)
        out.cleanup();
        return out;
    }

    static sparse_vec fft(const sparse_vec &x) {
        int n = x.len;
        if (n<=1) return x;

        // Split up into even and odd parts
        sparse_vec even(n/2);
        sparse_vec odd(n/2);
        for (auto duplet : x.duplets) {
            if (duplet.index % 2 == 0)
                even.append(duplet.index/2, duplet.value);
            else
                odd.append((duplet.index-1)/2, duplet.value);
        }
        
        // Recursively compute fft on even/odd parts
        sparse_vec evenFFT = fft(even);
        sparse_vec oddFFT = fft(odd);

        // Setup phase factor
        T omega = exp(-2.*PI/n*I);

        // Output vector            
        sparse_vec out(n);

        // combine results, this is O(n log^2(n) )
        // for (int k = 0; k < n/2; k++){
        //     out.append(k,       y_even.get_val(k) + exp(-2*PI*I*k/n) * y_odd.get_val(k));
        //     out.append(k + n/2, y_even.get_val(k) - exp(-2*PI*I*k/n) * y_odd.get_val(k));
        // }

        // FASTER O(n log n): do not use get_val (thus cleanup neither), however this naive solution is
        // incorrect: T omega = exp(-2.*(PI/n)*I); T s(1.0,0.0); // s = 1 + 0i for (int k = 0; k <
        // n/2; k++){
        //     out.append(k,       y_even.duplets[k].value + phase * y_odd.duplets[k].value);
        //     out.append(k + n/2, y_even.duplets[k].value - phase * y_odd.duplets[k].value);
        //     phase *= omega;
        // }

        // Lambda for abstracting efficient merge => O(n log n)
        auto efficientMerge = [&](int shift) { // catches automatically the ref to out, evenFFT, oddFFT, omega, s via [&].
            int evenCounter = 0;
            int oddCounter = 0;
            int evenSize = evenFFT.duplets.size();
            int oddSize = oddFFT.duplets.size();

            // while merging needed between even and odd parts
            while (evenCounter < evenSize && oddCounter < oddSize) {
                int evenIndex = evenFFT.duplets[evenCounter].index + shift;
                int oddIndex = oddFFT.duplets[oddCounter].index + shift;

                if (evenIndex == oddIndex) {
                    T tmp = 0; // Template support for T = complex<float/...> numbers
                    do {
                        tmp += evenFFT.duplets[evenCounter].value;
                        tmp += oddFFT.duplets[oddCounter].value * pow(omega, evenIndex);
                        evenCounter++;
                        oddCounter++;
                    } while (evenCounter < evenSize && oddCounter < oddSize &&
                            evenFFT.duplets[evenCounter].index == evenIndex &&
                            oddFFT.duplets[oddCounter].index == evenIndex); // only continue to do if duplets have elements with repeated indices
                    out.append(evenIndex, tmp); // add element to out sparse vector
                }
                else if (evenIndex < oddIndex) { // if un-paired even element
                    out.append(evenIndex, evenFFT.duplets[evenCounter].value);
                    evenCounter++;
                }
                else if (evenIndex > oddIndex) { // if un-paired odd element
                    out.append(oddIndex, oddFFT.duplets[oddCounter].value * pow(omega, oddIndex));
                    oddCounter++;
                }
            }

            // For left out elements (the even or odd part is done)
            while (evenCounter < evenSize) {
                int evenIndex = evenFFT.duplets[evenCounter].index + shift;
                out.append(evenIndex, evenFFT.duplets[evenCounter].value);
                evenCounter++;
            }
            while (oddCounter < oddSize)
            {
                int oddIndex = oddFFT.duplets[oddCounter].index + shift;
                out.append(oddIndex, oddFFT.duplets[oddCounter].value * pow(omega, oddIndex));
                oddCounter++;
            }
        }; // lambda def

        // Merge even and odd parts
        efficientMerge(0);
        efficientMerge(n/2);

        return out;
    }
};

int main()
{
    sparse_vec<complex<double>> x(5);
    x.append(3, complex<double>(8.2, 0));
    x.append(1, complex<double>(1, -2));
    x.append(4, complex<double>(-3, 4.66));  // this should be summed
    x.append(4, complex<double>(0, 4));      // this should be summed
    x.append(5, complex<double>(2, 3));      // this should be deleted

    x.cleanup();

    cout << "Printing values at all indices in the sparse vector: \n";
    for (int i = 0; i < x.len; i++)
    {
        cout << x.get_val(i) << endl;
    }
    cout << "\n Actual content in x.duplets: \n";
    for (auto d : x.duplets)
    {
        cout << d.index << " " << d.value << endl;
    }

    // Tests for cwise_mult
    sparse_vec<complex<double>> y(5);
    y.append(3, complex<double>(8.2, 0));
    y.append(1, complex<double>(1, -2));
    y.append(4, complex<double>(-3, 4.66));  // this should be summed
    y.append(4, complex<double>(0, 4));      // this should be summed
    y.cleanup();
    sparse_vec<complex<double>> z = sparse_vec<complex<double>>::cwise_mult(x, y);
    cout << "\n Printing values at all indices in the sparse vector z = x*y: \n";
    for (int i = 0; i < z.len; i++)
    {
        cout << z.get_val(i) << endl;
    }

    // Tests for conv
    sparse_vec<complex<double>> w = sparse_vec<complex<double>>::conv(x, y);
    cout << "\n Printing values at all indices in the sparse vector w = x(*)y: \n";
    for (int i = 0; i < w.len; i++)
    {
        cout << w.get_val(i) << endl;
    }

    // Tests for fft
    sparse_vec<complex<double>> v(8);
    v.append(0, complex<double>(1., 0.));
    v.append(1, complex<double>(2., 0.));
    v.append(2, complex<double>(3., 0.));
    v.append(3, complex<double>(4., 0.));
    v.append(4, complex<double>(5., 0.));
    v.append(5, complex<double>(6., 0.));
    v.append(6, complex<double>(7., 0.));
    v.append(7, complex<double>(8., 0.));
    // v.cleanup(); // v already cleaned

    cout << "\n Print all elements in v.duplets: \n";
    for (auto d : v.duplets)
    {
        cout << d.index << " " << d.value << endl;
    }

    sparse_vec<complex<double>> u = sparse_vec<complex<double>>::fft(v); // FIXME: THIS IS WRONG EVEN WHEN DIRECTLY TAKING THE PROVIDED SOLUTIONS
    cout << "\n Printing values at all indices in the sparse vector u = fft(v): \n";
        // u.cleanup();
    for (int i = 0; i < u.len; i++)
    {
        cout << u.get_val(i) << endl;
    }

    cout << "\n Printing all elements in u.duplets: \n";
    for (auto d : u.duplets)
    {
        cout << d.index << " " << d.value << endl;
    }

    return 0;
}
