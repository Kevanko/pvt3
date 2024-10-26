#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double eps = 1E-6;
const int n0 = 100;
const double a = -1;
const double b = 1;

double func(double x)
{
    return (sin(x + 2) / (0.4 + cos(x)));
}

double getrand()
{
    return (double)rand() / RAND_MAX;
}

int main(int argc, char **argv)
{
    int commsize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = n0, k;
    double sq[2], delta = 1;
    double start_time, end_time;

    if (rank == 0)
        start_time = MPI_Wtime();

    for (k = 0; delta > eps; n *= 2, k ^= 1) 
    {
        double h = (b - a) / n;
        double s = 0.0;
        for (int i = rank; i < n; i += commsize)
            s += func(a + h * (i + 0.5));

        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        sq[k] *= h;
        
        if (n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0; 
    }

    if (rank == 0)
    {
        end_time = MPI_Wtime(); 
        printf("Result Pi: %.12f; Runge rule: EPS %e, n %d\n", sq[k] * sq[k], eps, n / 2);
        printf("Time: %lf seconds\n", end_time - start_time);
    }
    MPI_Finalize();
    return 0;
}