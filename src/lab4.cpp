#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  const int message_size = 1024;
  MPI_Init(&argc, &argv);

  int commsize, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  char send_buffer[message_size];
  char recv_buffers[commsize][message_size];
  MPI_Request requests[2 * commsize - 1];
  MPI_Status statuses[2 * commsize - 1];

  for (int i = 0; i < message_size; i++) {
    send_buffer[i] = rand() % 128;
  }

  double start_time = MPI_Wtime();

  // Передача данных между процессами
  for (int j = 0; j < commsize; j++) {
    if (j == rank) {
      MPI_Isend(send_buffer, message_size, MPI_CHAR, j, 0,
                MPI_COMM_WORLD, &requests[j]);
      MPI_Irecv(recv_buffers[j], message_size, MPI_CHAR, j,
                0, MPI_COMM_WORLD, &requests[j + commsize]);
    } else {
      MPI_Isend(send_buffer, message_size, MPI_CHAR, j, 0,
                MPI_COMM_WORLD, &requests[j]);
      MPI_Irecv(recv_buffers[j], message_size, MPI_CHAR, j,
                0, MPI_COMM_WORLD, &requests[j + commsize]);
    }
  }

  // Ожидание завершения всех операций
  MPI_Waitall(2 * commsize - 1, requests, statuses);

  double end_time = MPI_Wtime();

  if (rank == 0) {
    printf("Размер сообщения = %d байт, время передачи = %f секунд\n",
           message_size, end_time - start_time);
  }

  MPI_Finalize();
  return 0;
}
