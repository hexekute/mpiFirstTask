#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char *data;
	int i;
	int n = atoi(argv[1]);
        int num_send = 1024 * 1024 * 20 / n + 1;

	data = (char*)malloc(n);

	double t1 = MPI_Wtime();
	for (i = 0; i < num_send; i++) {
		if (!rank) {
			MPI_Ssend(data, n, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(data, n, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		} else if (rank == 1) {
			MPI_Recv(data, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Ssend(data, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		}
	}
	double t2 = MPI_Wtime() - t1;
	free(data);
	if (rank == 0) {
		double t = t2 / num_send / 2.0;
		printf("%d, %f, %f\n", n, t * 1e6, n / t / 1024 / 1024);
	}

	MPI_Finalize();
	return 0;
}
