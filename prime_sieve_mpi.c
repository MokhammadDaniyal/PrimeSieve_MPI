/* program hello */
/* Adapted from mpihello.f by drs */

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int rank;
    char hostname[256];
    int limit;
    int number;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &limit);
    gethostname(hostname, 255);

    if (rank == 0)
    {
        printf("I am GENERATOR --- %d \n", rank);
        number = 2;

        while (1)
        {
            MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            number++;
        }
    }
    else
    {
        if (rank == limit - 1)
        {
            int has_first_number = 0;
            int first_number;
            while (1){

                if (has_first_number){
                    printf("Last prime recieved. Terminating...\n");
                    MPI_Abort(MPI_COMM_WORLD, 0);
                } else {
                    MPI_Recv(&number, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    printf("Process %d received number %d from process %d\n",rank, number, rank-1);
                    has_first_number = 1;
                    first_number = number;
                }
            }
        }
        else
        {
            int has_first_number = 0;
            int first_number;
            while (1)
            {
                MPI_Recv(&number, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (has_first_number){
                    if (number % first_number == 0){
                        continue;
                    } else {
                    MPI_Ssend(&number, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
                    }
                }else {
                    has_first_number = 1;
                    printf("Process %d received number %d from process %d\n",rank, number, rank-1);
                    first_number = number;
                }
            }
            // printf("I am MIDDLEWARE --- %d \n", rank);
        }
    }
    MPI_Finalize();

    return 0;
}