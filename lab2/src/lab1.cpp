#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double func(double x) 
{
return (sqrt(x*(3 - x))) / (x + 1);
}

int main(int argc, char **argv)
{
    double start_time;
    double end_time;
    const double a = 1;
    const double b = 1.2;
    const int n0 = 100;
    const double eps = 1E-6;
    int commsize, rank;
    int n = n0, k;
    double sq[2], delta = 1;
    int count = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == 0)
        start_time = MPI_Wtime();
    for (k = 0; delta > eps; n *= 2, k ^= 1) 
    {
        //int points_per_proc = n / commsize;
        // int lb = rank * points_per_proc;
        // int ub = (rank == commsize - 1) ? (n - 1) : (lb + points_per_proc - 1);
        double h = (b - a) / n;
        double s = 0.0;
        //for (int i = lb; i <= ub; i++)
        for (int i = rank; i < n; i += commsize)
            s += func(a + h * (i + 0.5));

        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        sq[k] *= h;
        if (n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
        count++;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    ;
    if (rank == 0) 
    {
        end_time = MPI_Wtime();
        printf("Result: %.12f EPS: %e, n: %d count: %d\n", sq[k], eps, n / 2, count);
        printf("Time: %lf\n", end_time - start_time);
    }
    MPI_Finalize();
    return 0;
}