#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	int rank, size;

	char *data;
	int n = atoi(argv[1]);
	int num_send = 1024 * 1024 * 20 / n + 1;
	int i;
	char *bufmpi;
	int bsize;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Pack_size(n, MPI_CHAR, MPI_COMM_WORLD, &bsize);
	bufmpi = (char*)malloc(bsize + MPI_BSEND_OVERHEAD);
	data = (char*)malloc(n);
	MPI_Buffer_attach(bufmpi, bsize + MPI_BSEND_OVERHEAD);

	double t1 = MPI_Wtime();
	for (i = 0; i < num_send; ++i) {
		if (!rank) {
			MPI_Bsend(data, n, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(data, n, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}else{
			MPI_Recv(data, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Bsend(data, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		}
	}

	double t2 = MPI_Wtime() - t1;
	free(data);
     
     	if (!rank) {
		double t = t2 / num_send / 2.0;
		printf("%d, %f, %f\n", n, t * 1e6, n / t / 1024 / 1024);
    	}

        MPI_Buffer_detach(&bufmpi, &bsize);
        free(bufmpi);
        MPI_Finalize();
        return 0;
} 
