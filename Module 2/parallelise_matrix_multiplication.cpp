#include <iostream>
#include <chrono>
#include <random>
#include <pthread.h>

const int N = 1000;
const int M = 100; // size of submatrices

const int num_threads = 8; // number of threads
double a[N*N], b[N*N], c[N*N];

void init_matrix(double* matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    for (int i = 0; i < N*N; i++) {
        matrix[i] = dis(gen);
    }
}

void* matrix_multiply_thread(void* arg) {
    int start_row = *((int*) arg);
    int end_row = start_row + M;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += a[i*N + k] * b[k*N + j];
            }
            c[i*N + j] = sum;
        }
    }

    pthread_exit(NULL);
}

int main() {
    init_matrix(a);
    init_matrix(b);

    // Initialize each submatrix of C to 0
    for (int i = 0; i < N*N; i += M*M) {
        for (int j = 0; j < M*M; j++) {
            c[i + j] = 0.0;
        }
    }

    pthread_t threads[num_threads];
    int thread_args[num_threads];

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; i++) {
        thread_args[i] = i * M; // starting row of submatrix
        pthread_create(&threads[i], NULL, matrix_multiply_thread, &thread_args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Execution time: " << duration.count() / 1000000.0 << " seconds" << std::endl;

    return 0;
}
