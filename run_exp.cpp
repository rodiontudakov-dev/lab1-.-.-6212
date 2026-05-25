#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <omp.h>
#include <string>
#include <random>
#include <cmath>

using namespace std;
using namespace chrono;

vector<vector<double>> generateMatrix(int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);
    
    vector<vector<double>> mat(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = dist(gen);
        }
    }
    return mat;
}

vector<vector<double>> multiplyMatrices(const vector<vector<double>>& A, 
                                         const vector<vector<double>>& B) {
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n, 0.0));
    
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            double aik = A[i][k];
            for (int j = 0; j < n; j++) {
                C[i][j] += aik * B[k][j];
            }
        }
    }
    
    return C;
}

double runTest(int n, int threads) {
    omp_set_num_threads(threads);
    
    auto A = generateMatrix(n);
    auto B = generateMatrix(n);
    
    auto start = high_resolution_clock::now();
    auto C = multiplyMatrices(A, B);
    auto end = high_resolution_clock::now();
    
    duration<double> elapsed = end - start;
    return elapsed.count();
}

int main() {
    vector<int> sizes = {200, 400, 800, 1200, 1600, 2000};
    vector<int> threads_list = {1, 2, 4, 8};
    int runs = 3;
    
    ofstream out("experiment_results.csv");
    out << "N;threads;run;time_seconds\n";
    
    for (int n : sizes) {
        for (int threads : threads_list) {
            for (int run = 1; run <= runs; run++) {
                cout << "Testing: N=" << n << " threads=" << threads << " run=" << run << " ... " << flush;
                
                double time = runTest(n, threads);
                
                out << n << ";" << threads << ";" << run << ";" << fixed << setprecision(6) << time << "\n";
                cout << time << " sec" << endl;
            }
        }
    }
    
    out.close();
    
    cout << "\nDone! Results saved to experiment_results.csv" << endl;
    
    return 0;
}