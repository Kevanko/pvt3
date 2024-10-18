#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, commsize;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int message_size = 1024;

    for (int iteration = 0; iteration < 2; iteration++) {
        // Буфер для отправки
        char send_buffer[message_size];
        // Инициализируем буфер случайными значениями
        for (int i = 0; i < message_size; i++) {
            send_buffer[i] = rand() % 128;
        }

        // Буфер для приема данных на процессе 0
        char receive_buffer[commsize][message_size];

        // Засекаем время начала обмена
        double start_time = MPI_Wtime();

        if (rank == 0) {
            // Процесс с рангом 0 получает данные от всех других процессов
            for (int src_rank = commsize - 1; src_rank > 0; src_rank--) {
                MPI_Recv(receive_buffer[src_rank], cmessage_size, MPI_CHAR, src_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else {
            // Остальные процессы отправляют данные на процесс 0
            MPI_Send(send_buffer, message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }

        // Засекаем время окончания обмена
        double end_time = MPI_Wtime();

        // Выводим результаты для процесса с рангом 0
        if (rank == 0) {
            printf("Размер сообщения = %d байт, время передачи = %f секунд\n", message_size, end_time - start_time);
        }

        // Увеличиваем размер буфера для следующей итерации
        message_size *= 1024;
    }

    MPI_Finalize();
    return 0;
}
