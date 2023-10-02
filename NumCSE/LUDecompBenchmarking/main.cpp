#include <cassert>
#include <iostream>
#include <vector>
#include <Eigen/Eigen>

#define TimeNow std::chrono::high_resolution_clock::now
#define TimeVar std::chrono::time_point<std::chrono::high_resolution_clock>

using std::cout;
using std::endl;
using std::vector;
using Eigen::Triplet;
using std::function;
using std::chrono::duration;


/// @brief Function that makes the triplet list for a tridiagonal matrix
/// @param n is the size of the matrix
/// @return the triplet list
vector<Triplet<double>> makeTripletList(int n){
    // Make triplet list for tridiagonal matrix
    int nnz = 3 * n - 2;
    vector<Triplet<double>> tripletList;
    tripletList.reserve(nnz);
    for (int i = 0; i < n - 1; ++i){
        tripletList.push_back(Triplet<double>(i, i + 1, -1.0)); // lower diagonal entry
        tripletList.push_back(Triplet<double>(i, i, 2.0)); // diagonal entry
        tripletList.push_back(Triplet<double>(i + 1, i, -1.0)); // upper diagonal entry
    }
    tripletList.push_back(Triplet<double>(n - 1, n - 1, 2.0)); // last diagonal entry
    return tripletList;
}


/// @brief Function to measure runtime of a function
/// @param func is the function to be measured
/// @return The average runtime of the function (10 reps.)
double runTime(const function<void(void)> &func){
    TimeVar start, end;
    int reps = 10; // number of repetitions
    double sum = 0.0; // sum of runtimes

    for (int i = 0; i < reps; ++i){
        start = TimeNow();
        func();
        end = TimeNow();
        sum += duration<double>(end - start).count();
    }
    return sum / reps;
}


int main(){
    // Initialize matrix sizes
    vector<int> nList = {16, 32, 64, 128, 256, 512};
    cout << "LU decomposition of tridiagonal matrix" << endl;
    cout << "=======================================" << endl;
    cout << "n" << "\t\t" << "SparseLU time (s)" << "\t" << "DenseLU time (s)" << endl;

    vector<double> timeSparse;
    vector<double> timeDense;

    for (int n : nList) {
        auto tripletList = makeTripletList(n);

        // Sparse matrix
        Eigen::SparseMatrix<double> A(n, n);
        A.setFromTriplets(tripletList.begin(), tripletList.end());

        Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
        function<void(void)> funcSparse = [&](){solver.compute(A);};

        // Dense matrix
        Eigen::MatrixXd B(A);
        Eigen::FullPivLU<Eigen::MatrixXd> solverDense;
        function<void(void)> funcDense = [&](){solverDense.compute(B);};

        // Measure runtime
        timeSparse.push_back(runTime(funcSparse));
        timeDense.push_back(runTime(funcDense));
    }

    // Print results
    for (int i = 0; i < nList.size(); ++i){
        cout << nList[i] << "\t\t" << timeSparse[i] << "\t\t" << timeDense[i] << endl;
    }


    return 0;
}