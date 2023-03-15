#include <iostream>
#include <chrono>
#include <random>

const int N = 1000;

void init_matrix(double* matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    for (int i = 0; i < N*N; i++) {
        matrix[i] = dis(gen);
    }
}

void matrix_multiply(double* a, double* b, double* c) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += a[i*N + k] * b[k*N + j];
            }
            c[i*N + j] = sum;
        }
    }
}

int main() {
    double* a = new double[N*N];
    double* b = new double[N*N];
    double* c = new double[N*N];

    init_matrix(a);
    init_matrix(b);

    auto start_time = std::chrono::high_resolution_clock::now();

    matrix_multiply(a, b, c);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Execution time: " << duration.count() / 1000000.0 << " seconds" << std::endl;

    delete[] a;
    delete[] b;
    delete[] c;
    return 0;
}
