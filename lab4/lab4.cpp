#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 0.001
#define PI 3.14159265358979323846
#define IND(i, j, nx) ((i) * (nx + 2) + (j))

// Функция для вычисления размера блока
int get_block_size(int n, int rank, int nprocs) {
    int size = n / nprocs;
    if (n % nprocs > rank) size++;
    return size;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int commsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int dims[2] = {0, 0}, periodic[2] = {0, 0};
    MPI_Dims_create(commsize, 2, dims); // Разбиение процессов в 2D-решетку
    int px = dims[0], py = dims[1];

    MPI_Comm cartcomm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periodic, 0, &cartcomm);

    // Координаты процесса в 2D-решетке
    int coords[2];
    MPI_Cart_coords(cartcomm, rank, 2, coords);
    int rankx = coords[0], ranky = coords[1];

    // Размеры локальной сетки
    int nx = get_block_size(atoi(argv[1]), rankx, px);
    int ny = get_block_size(atoi(argv[2]), ranky, py);

    // Выделение памяти для локальных сеток
    double *grid = calloc((ny + 2) * (nx + 2), sizeof(double));
    double *newgrid = calloc((ny + 2) * (nx + 2), sizeof(double));

    // Граничные условия
    double dx = 1.0 / (atoi(argv[1]) - 1);
    double dy = 1.0 / (atoi(argv[2]) - 1);

    if (ranky == 0) { // Верхняя граница
        for (int j = 1; j <= nx; j++) {
            double x = dx * (j - 1);
            grid[IND(0, j, nx)] = sin(PI * x);
        }
    }
    if (ranky == py - 1) { // Нижняя граница
        for (int j = 1; j <= nx; j++) {
            double x = dx * (j - 1);
            grid[IND(ny + 1, j, nx)] = sin(PI * x) * exp(-PI);
        }
    }

    // Метод Якоби с обменом граничных данных
    int niters = 0;
    double maxdiff;
    do {
        niters++;
        maxdiff = 0.0;

        // Обновление внутренних точек
        for (int i = 1; i <= ny; i++) {
            for (int j = 1; j <= nx; j++) {
                newgrid[IND(i, j, nx)] = 0.25 * (
                    grid[IND(i - 1, j, nx)] +
                    grid[IND(i + 1, j, nx)] +
                    grid[IND(i, j - 1, nx)] +
                    grid[IND(i, j + 1, nx)]
                );
            }
        }

        // Вычисление разности для критерия завершения
        for (int i = 1; i <= ny; i++) {
            for (int j = 1; j <= nx; j++) {
                double diff = fabs(grid[IND(i, j, nx)] - newgrid[IND(i, j, nx)]);
                if (diff > maxdiff) maxdiff = diff;
            }
        }

        // Обмен граничными строками и столбцами
        MPI_Allreduce(MPI_IN_PLACE, &maxdiff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

        // Обновление сетки
        double *temp = grid;
        grid = newgrid;
        newgrid = temp;

    } while (maxdiff > EPS);

    if (rank == 0) {
        printf("Iterations: %d\n", niters);
    }

    free(grid);
    free(newgrid);
    MPI_Finalize();
    return 0;
}
