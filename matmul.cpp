#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <omp.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace chrono;

void setCpuAffinity(int cores) {
#ifdef _WIN32
    HANDLE proc = GetCurrentProcess();
    DWORD_PTR mask = 0;
    for (int i = 0; i < cores; i++) {
        mask |= (1ULL << i);
    }
    SetProcessAffinityMask(proc, mask);
#endif
}

vector<vector<double>> loadMatrix(const string& path, int& size) {
    ifstream file(path);
    if (!file) {
        cerr << "Ошибка: не удалось открыть " << path << endl;
        exit(1);
    }

    file >> size;
    vector<vector<double>> mat(size, vector<double>(size));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            file >> mat[i][j];
        }
    }

    file.close();
    return mat;
}

void saveMatrix(const string& path, const vector<vector<double>>& mat) {
    ofstream file(path);
    int n = mat.size();
    file << n << "\n";
    file << fixed << setprecision(6);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file << mat[i][j] << " ";
        }
        file << "\n";
    }

    file.close();
}

vector<vector<double>> multiplyMatricesParallel(const vector<vector<double>>& A,
    const vector<vector<double>>& B) {
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n, 0.0));

#pragma omp parallel for schedule(dynamic)
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

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cout << "Использование: " << argv[0] << " <файл_A> <файл_B> <файл_результата> <потоки>" << endl;
        return 1;
    }

    string fileA = argv[1];
    string fileB = argv[2];
    string fileRes = argv[3];
    int threads = stoi(argv[4]);

    omp_set_num_threads(threads);

    cout << "\n==================================================" << endl;
    cout << "  ПАРАЛЛЕЛЬНОЕ УМНОЖЕНИЕ МАТРИЦ" << endl;
    cout << "==================================================" << endl;
    cout << "  Потоков: " << threads << endl;
    cout << "==================================================\n" << endl;

    int n;
    auto A = loadMatrix(fileA, n);
    auto B = loadMatrix(fileB, n);

    cout << "Размер матриц: " << n << " x " << n << endl;

    long long memUsage = 3LL * n * n * sizeof(double);
    cout << "Память: ~" << memUsage / (1024.0 * 1024.0) << " MB" << endl;

    long long totalOps = 2LL * n * n * n;
    cout << "Операций: " << totalOps << endl;

    auto start = high_resolution_clock::now();
    auto C = multiplyMatricesParallel(A, B);
    auto end = high_resolution_clock::now();

    double elapsed = duration<double>(end - start).count();
    double gflops = (totalOps / 1e9) / elapsed;

    cout << "\n==================================================" << endl;
    cout << "  РЕЗУЛЬТАТЫ" << endl;
    cout << "==================================================" << endl;
    cout << "  Время: " << elapsed << " сек" << endl;
    cout << "  Производительность: " << gflops << " GFLOPS" << endl;
    cout << "==================================================\n" << endl;

    saveMatrix(fileRes, C);

    ofstream report("performance.log");
    report << "=== ОТЧЕТ ===" << endl;
    report << "Размер: " << n << endl;
    report << "Потоков: " << threads << endl;
    report << "Время: " << elapsed << " сек" << endl;
    report << "Операций: " << totalOps << endl;
    report << "GFLOPS: " << gflops << endl;
    report.close();

    cout << "Результат: " << fileRes << endl;
    cout << "Отчёт: performance.log" << endl;

    return 0;
}