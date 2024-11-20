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
    for (int a = 0; a < n; a++) {
      sbuf[a] = rand() % 128;
    }
    char *rbuf = (char*)malloc(commsize * n);
    double start_t = MPI_Wtime();
    if (rank == 0) {
      for (int j = 1; j < commsize; j++) {
        MPI_Recv(&rbuf[j*commsize], n, MPI_CHAR, j, 0, MPI_COMM_WORLD, 0);
      }
    } else {
      MPI_Send(&sbuf, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    double end_t = MPI_Wtime();
    if (rank == 0) {
      printf("Размер сообщения = %d байт, время передачи = %f секунд\n", n,
             end_t - start_t);
    }
    n *= 1024;
  }
  MPI_Finalize();
  return 0;
}
