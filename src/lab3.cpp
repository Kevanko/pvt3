#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int process_rank, total_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &total_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  int buffer_size = 1024;

  for (int iteration = 0; iteration < 2; iteration++) {
    // Буфер для отправки
    char send_buffer[buffer_size];
    // Инициализируем буфер случайными значениями
    for (int i = 0; i < buffer_size; i++) {
      send_buffer[i] = rand() % 128;
    }

    // Буфер для приема данных на процессе 0
    char receive_buffer[total_processes][buffer_size];

    // Засекаем время начала обмена
    double start_time = MPI_Wtime();

    if (process_rank == 0) {
      // Процесс с рангом 0 получает данные от всех других процессов
      for (int src_rank = total_processes - 1; src_rank > 0; src_rank--) {
        MPI_Recv(receive_buffer[src_rank], buffer_size, MPI_CHAR, src_rank, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    } else {
      // Остальные процессы отправляют данные на процесс 0
      MPI_Send(send_buffer, buffer_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    // Засекаем время окончания обмена
    double end_time = MPI_Wtime();

    // Выводим результаты для процесса с рангом 0
    if (process_rank == 0) {
      printf(
          "Buffer size = %d, Start time = %f, End time = %f, Time taken = %f\n",
          buffer_size, start_time, end_time, end_time - start_time);
    }

    // Увеличиваем размер буфера для следующей итерации
    buffer_size *= 1024;
  }

  MPI_Finalize();
  return 0;
}
