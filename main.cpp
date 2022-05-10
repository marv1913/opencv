#include <mpi.h>

#define SIZE 4
#define ROOT 0
#define SRC 1

void point_to_point(int argc, char **argv) {
    int size;
    int rank;
    int buffer;
    int sum = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Recv(&buffer, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += buffer;
        }
    } else {
        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        std::cout << "Total sum: " << sum << std::endl;
    }
    MPI_Finalize();
}

void print_ranks(int argc, char **argv) {
    int size;
    int rank;
    char name[128];
    int len;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &len);
    std::cout << "Hello from <" << rank << "> of <" << size << "> in " << name << std::endl;
    MPI_Finalize();
}

void scattering_and_gathering(int argc, char **argv) {
    int numtasks, rank, sendcount, recvcount, source;
    float sendbuf[SIZE][SIZE] = {
            {1.0,  2.0,  3.0,  4.0},
            {5.0,  6.0,  7.0,  8.0},
            {9.0,  10.0, 11.0, 12.0},
            {13.0, 14.0, 15.0, 16.0}};
    float recvbuf[SIZE];
    float recvbuf2[SIZE][SIZE];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    if (numtasks == SIZE) {
        source = SRC;
        MPI_Scatter(sendbuf, SIZE, MPI_FLOAT, recvbuf, SIZE,
                    MPI_FLOAT, source, MPI_COMM_WORLD);
        printf("Scattered row into process %d: %f %f %f %f\n",
               rank, recvbuf[0], recvbuf[1], recvbuf[2], recvbuf[3]);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Gather(&recvbuf, SIZE, MPI_FLOAT, recvbuf2, SIZE,
                   MPI_FLOAT, ROOT, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == ROOT) {
            printf("\nGathered all rows into one in ROOT:\n");
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    printf("%f ", recvbuf2[i][j]);
                }
                printf("\n");
            }
        }
    }
    MPI_Finalize();
}

int main(int argc, char **argv) {
//    print_ranks(argc, argv);
//    point_to_point(argc, argv);
    scattering_and_gathering(argc, argv);
}