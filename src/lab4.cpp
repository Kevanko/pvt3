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

  // Инициализируем буфер случайными значениями
  for (int i = 0; i < message_size; i++) {
    send_buffer[i] = rand() % 128;
  }

  // Замеряем время начала
  double start_time = MPI_Wtime();

  // Передача данных между процессами
  for (int target_rank = 0; target_rank < commsize; target_rank++) {
    if (target_rank == rank) {
      MPI_Isend(send_buffer, message_size, MPI_CHAR, target_rank, 0,
                MPI_COMM_WORLD, &requests[target_rank]);
      MPI_Irecv(recv_buffers[target_rank], message_size, MPI_CHAR, target_rank,
                0, MPI_COMM_WORLD, &requests[target_rank + commsize]);
    } else {
      MPI_Isend(send_buffer, message_size, MPI_CHAR, target_rank, 0,
                MPI_COMM_WORLD, &requests[target_rank]);
      MPI_Irecv(recv_buffers[target_rank], message_size, MPI_CHAR, target_rank,
                0, MPI_COMM_WORLD, &requests[target_rank + commsize]);
    }
  }

  // Ожидание завершения всех операций
  MPI_Waitall(2 * commsize - 1, requests, statuses);

  // Замеряем время окончания
  double end_time = MPI_Wtime();

  // Вывод результатов для процесса с рангом 0
  if (rank == 0) {
    printf("Размер сообщения = %d байт, время передачи = %f секунд\n",
           message_size, end_time - start_time);
  }

  MPI_Finalize();
  return 0;
}
