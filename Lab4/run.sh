#!/bin/bash
nvcc -O3 -arch=sm_75 matrix_cuda.cu -o matrix_cuda

./matrix_cuda 256 8
./matrix_cuda 256 16
./matrix_cuda 256 32

./matrix_cuda 512 8
./matrix_cuda 512 16
./matrix_cuda 512 32

./matrix_cuda 1024 8
./matrix_cuda 1024 16
./matrix_cuda 1024 32

./matrix_cuda 2048 16
./matrix_cuda 2048 32
