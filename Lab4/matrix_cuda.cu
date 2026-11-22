#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <cuda_runtime.h>

// CUDA-ядро для умножения матриц C = A * B
__global__ void matrixMulCUDA(const float* A, const float* B, float* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        float sum = 0.0f;
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

// Последовательное умножение на CPU для проверки корректности
void matrixMulCPU(const std::vector<float>& A, const std::vector<float>& B, std::vector<float>& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

int main(int argc, char* argv[]) {
    int N = (argc > 1) ? std::atoi(argv[1]) : 512;
    int BLOCK_SIZE = (argc > 2) ? std::atoi(argv[2]) : 16;

    size_t bytes = N * N * sizeof(float);
    std::vector<float> h_A(N * N), h_B(N * N), h_C(N * N), h_C_ref(N * N);

    for (int i = 0; i < N * N; ++i) {
        h_A[i] = static_cast<float>(rand()) / RAND_MAX;
        h_B[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, bytes);
    cudaMalloc(&d_B, bytes);
    cudaMalloc(&d_C, bytes);

    cudaMemcpy(d_A, h_A.data(), bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B.data(), bytes, cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 numBlocks((N + BLOCK_SIZE - 1) / BLOCK_SIZE, (N + BLOCK_SIZE - 1) / BLOCK_SIZE);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Прогрев GPU
    matrixMulCUDA<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, N);
    cudaDeviceSynchronize();

    // Основной замер времени на GPU
    cudaEventRecord(start);
    matrixMulCUDA<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, N);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float gpu_time_ms = 0;
    cudaEventElapsedTime(&gpu_time_ms, start, stop);

    cudaMemcpy(h_C.data(), d_C, bytes, cudaMemcpyDeviceToHost);

    double cpu_time_ms = 0;
    bool verified = true;
    if (N <= 1024) {
        auto t1 = std::chrono::high_resolution_clock::now();
        matrixMulCPU(h_A, h_B, h_C_ref, N);
        auto t2 = std::chrono::high_resolution_clock::now();
        cpu_time_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

        for (int i = 0; i < N * N; ++i) {
            if (std::fabs(h_C[i] - h_C_ref[i]) > 1e-2) {
                verified = false;
                break;
            }
        }
    }

    std::cout << "Matrix: " << N << "x" << N
              << " | Block: " << BLOCK_SIZE << "x" << BLOCK_SIZE
              << " | Grid: " << numBlocks.x << "x" << numBlocks.y
              << " | GPU Time: " << gpu_time_ms << " ms";
    if (N <= 1024) {
        std::cout << " | CPU Time: " << cpu_time_ms << " ms"
                  << " | Speedup: " << (cpu_time_ms / gpu_time_ms) << "x"
                  << " | Valid: " << (verified ? "YES" : "NO");
    }
    std::cout << std::endl;

    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    cudaEventDestroy(start); cudaEventDestroy(stop);
    return 0;
}
