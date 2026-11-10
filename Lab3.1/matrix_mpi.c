#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Чтение матрицы из текстового файла (только master)
double* read_matrix(const char* filename, int n) {
    FILE* f = fopen(filename, "r");
    if (!f) { perror("fopen"); MPI_Abort(MPI_COMM_WORLD, 1); }
    double* mat = (double*)malloc(n * n * sizeof(double));
    for (int i = 0; i < n * n; i++) {
        if (fscanf(f, "%lf", &mat[i]) != 1) {
            fprintf(stderr, "Error reading matrix %s at element %d\n", filename, i);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    fclose(f);
    return mat;
}

void write_matrix(const char* filename, double* mat, int n) {
    FILE* f = fopen(filename, "w");
    if (!f) { perror("fopen"); MPI_Abort(MPI_COMM_WORLD, 1); }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(f, "%.12e ", mat[i * n + j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 4) {
        if (rank == 0) fprintf(stderr, "Usage: %s N fileA fileB [fileC]\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);
    const char* fileA = argv[2];
    const char* fileB = argv[3];
    const char* fileC = (argc > 4) ? argv[4] : "result.txt";

    double *A = NULL, *B = NULL, *C = NULL;

    if (rank == 0) {
        A = read_matrix(fileA, n);
        B = read_matrix(fileB, n);
        C = (double*)malloc(n * n * sizeof(double));
    } else {
        B = (double*)malloc(n * n * sizeof(double));
    }

    // Все процессы получают матрицу B
    MPI_Bcast(B, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Вычисляем, сколько строк достанется каждому процессу
    int base_rows = n / size;
    int remainder = n % size;
    int my_rows = base_rows + (rank < remainder ? 1 : 0);
    int my_offset = rank * base_rows + (rank < remainder ? rank : remainder);

    double* local_A = (double*)malloc(my_rows * n * sizeof(double));
    double* local_C = (double*)malloc(my_rows * n * sizeof(double));

    // Master рассылает блоки строк A
    if (rank == 0) {
        // Свой блок — копируем напрямую
        memcpy(local_A, A + my_offset * n, my_rows * n * sizeof(double));
        // Остальным — отправляем
        for (int p = 1; p < size; p++) {
            int p_rows = base_rows + (p < remainder ? 1 : 0);
            int p_offset = p * base_rows + (p < remainder ? p : remainder);
            MPI_Send(A + p_offset * n, p_rows * n, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(local_A, my_rows * n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Замер времени (только master)
    double start_time = MPI_Wtime();

    // Вычисление local_C = local_A * B
    for (int i = 0; i < my_rows; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += local_A[i * n + k] * B[k * n + j];
            }
            local_C[i * n + j] = sum;
        }
    }

    double end_time = MPI_Wtime();

    // Master собирает результаты
    if (rank == 0) {
        memcpy(C + my_offset * n, local_C, my_rows * n * sizeof(double));
        for (int p = 1; p < size; p++) {
            int p_rows = base_rows + (p < remainder ? 1 : 0);
            int p_offset = p * base_rows + (p < remainder ? p : remainder);
            MPI_Recv(C + p_offset * n, p_rows * n, MPI_DOUBLE, p, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("N=%d, procs=%d, time=%.6f sec\n", n, size, end_time - start_time);
        write_matrix(fileC, C, n);
    } else {
        MPI_Send(local_C, my_rows * n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    if (rank == 0) { free(A); free(C); }
    free(B);
    free(local_A);
    free(local_C);
    MPI_Finalize();
    return 0;
}