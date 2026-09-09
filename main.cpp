#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

int main() {
    cout << "========== MATRIX MULTIPLICATION ==========" << endl;
    
    ifstream fileA("matrix_a.txt");
    ifstream fileB("matrix_b.txt");
    
    if (!fileA.is_open() || !fileB.is_open()) {
        cout << "Error: failed to open files!" << endl;
        return 1;
    }
    
    int n;
    fileA >> n;
    fileB >> n;
    
    cout << "Matrix size: " << n << "x" << n << endl;
    
    vector<vector<double>> A(n, vector<double>(n));
    vector<vector<double>> B(n, vector<double>(n));
    vector<vector<double>> C(n, vector<double>(n, 0));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fileA >> A[i][j];
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fileB >> B[i][j];
        }
    }
    
    fileA.close();
    fileB.close();
    
    auto start = high_resolution_clock::now();
    
    cout << "Multiplying matrices..." << endl;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    double time_sec = duration.count() / 1000.0;
    
    cout << "Execution time: " << time_sec << " seconds" << endl;
    
    ofstream resultFile("result_cpp.txt");
    resultFile << n << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            resultFile << fixed << setprecision(15) << C[i][j] << " ";
        }
        resultFile << endl;
    }
    resultFile.close();
    
    cout << "Result saved to result_cpp.txt" << endl;
    
    long long memory = 3 * n * n * sizeof(double);
    long long operations = 2LL * n * n * n;
    
    cout << "\n========== METRICS ==========" << endl;
    cout << "Memory: " << memory / 1024 << " KB" << endl;
    cout << "Operations: " << operations << endl;
    if (time_sec > 0) {
        cout << "Performance: " << (operations / 1e9) / time_sec << " GFLOPS" << endl;
    } else {
        cout << "Performance: time too small to measure" << endl;
    }
    
    cout << "\nRunning verification..." << endl;
    system("py verify.py");
    
    return 0;
}