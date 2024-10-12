#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int n = 1;
    int msg_tag;
    int commsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < 3; i++) { 
        char *msg = (char*)malloc(n * sizeof(char));
        if (msg == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (int j = 0; j < n; j++) {
            msg[j] = rand() % 128;
        }

        msg_tag = rank; 
        MPI_Status stat;
        double start_time = MPI_Wtime(); 

        for (int k = 0; k < commsize; k++) {
            int dest = (rank + 1) % commsize;   
            int source = (rank - 1 + commsize) % commsize; 
            
            MPI_Sendrecv(msg, n, MPI_CHAR, dest, msg_tag, 
                         msg, n, MPI_CHAR, source, MPI_ANY_TAG, 
                         MPI_COMM_WORLD, &stat);
        }

        double end_time = MPI_Wtime(); 
        
        if (rank == 0) {
            printf("Message size: %d , Time = %f\n", 
                   n, start_time, end_time, end_time - start_time);
        }

        n *= 1024;

        free(msg);
    }

    MPI_Finalize();
    return 0;
}
