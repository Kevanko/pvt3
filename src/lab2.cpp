#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int rank, commsize;
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 1024;
  for (int i = 0; i <= 1; i++) {
    char sbuf[n];
    for (int b = 0; b < n; b++) {
      sbuf[b] = rand() % 128;
    }
    char rbuf[n];
    double start_t = MPI_Wtime();
    if (rank == 0) {
      for (int j = 1; j < commsize; j++) {
        MPI_Send(&sbuf, n, MPI_CHAR, j, 0, MPI_COMM_WORLD);
      }
    } else {
      MPI_Recv(&rbuf, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, 0);
    }
    double end_t = MPI_Wtime();
    if (rank == 0)
      printf("Размер сообщения = %d байт, время передачи = %f секунд\n", n,
             end_t - start_t);

    n *= 1024;
  }
  MPI_Finalize();
  return 0;
}