#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, commsize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  // Проверка конфигурации
  if (commsize != 32) {
    if (rank == 0) {
      printf("Запуск программы должен быть на 32 процессах.\n");
    }
    MPI_Finalize();
    return 1;
  }

  // Размеры сообщений: 1 KB и 1 MB
  int sizes[] = {1024, 1024 * 1024};
  char *buffer;

  for (int i = 0; i < 2; ++i) {
    int message_size = sizes[i];
    buffer = (char *)malloc(message_size * sizeof(char));

    // Инициализация буфера для процесса 0
    if (rank == 0) {
      for (int j = 0; j < message_size; ++j) {
        buffer[j] = 'a'; // Заполнение буфера
      }
    }

    // Замер времени
    double start_time = MPI_Wtime();

    // Трансляция сообщения от процесса 0 ко всем остальным процессам
    MPI_Bcast(buffer, message_size, MPI_CHAR, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
      printf("Размер сообщения = %d байт, время передачи = %f секунд\n",
             message_size, end_time - start_time);
    }

    free(buffer);
  }

  MPI_Finalize();
  return 0;
}
