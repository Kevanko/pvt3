#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void initialize_data(float *a, float *b, int m, int n, int lb, int ub) {
    for (int i = 0; i < (ub - lb); i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = (lb + i + 1) * (j + 1);
        }
    }
    for (int j = 0; j < n; j++) {
        b[j] = j + 1;
    }
}

void sgemv(float *a, float *b, float *c, int m, int n, int lb, int ub) {
    for (int i = lb; i < ub; i++) {
        c[i] = 0.0f;
        for (int j = 0; j < n; j++) {
            c[i] += a[(i - lb) * n + j] * b[j];
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int m = 28000, n = 2800; // Размеры матрицы
    int rows_per_proc = m / size;
    int lb = rank * rows_per_proc;
    int ub = (rank == size - 1) ? m : lb + rows_per_proc;

    float *a = (float *)malloc((ub - lb) * n * sizeof(float));
    float *b = (float *)malloc(n * sizeof(float));
    float *c = (float *)malloc(m * sizeof(float));

    initialize_data(a, b, m, n, lb, ub);

    double start_time = MPI_Wtime();
    sgemv(a, b, c, m, n, lb, ub);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Execution time: %f seconds\n", end_time - start_time);
    }

    free(a);
    free(b);
    free(c);
    MPI_Finalize();
    return 0;
}
