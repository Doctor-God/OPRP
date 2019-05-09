#include <complex>
#include <iostream>
#include <omp.h>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv){
	int max_row, max_column, max_n, piece, my_max, tag=0;
	int size, rank;
	MPI_Status status;
	// cin >> max_row;
	// cin >> max_column;
	// cin >> max_n;


	MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	max_row = atoi(argv[1]);
	max_column = atoi(argv[2]);
	max_n = atoi(argv[3]);
	
	piece = (max_row*max_column)/size;
	if(rank == size-1){
		my_max = max_row*max_column;
	}
	else{
		my_max = (rank+1)*piece;
	}

	char **mat = (char **)malloc(sizeof(char *) * max_row);

	char* bloco = (char*) malloc(sizeof(char)*max_column*max_row);

	for (int i = 0; i < max_row; i++)
		mat[i] = bloco + (i*max_column);


	// for(int p = 0; p < size-1; p++){
	// 	MPI_Send(bloco + p*piece, piece, MPI_CHAR, p, tag, MPI_COMM_WORLD);
	// }
	// MPI_Send(bloco + (size-1)*piece, max_row*max_column - (size-1)*piece, MPI_CHAR, size-1, tag, MPI_COMM_WORLD);


	for (int f = rank*piece; f < my_max; ++f)
	{
		int r = f % max_column;
		int c = f - r*max_column;

		cout << "oi " << rank << " " << r << " " << f << endl;

		complex<float> z;
		int n = 0;
		while (abs(z) < 2 && ++n < max_n)
			z = pow(z, 2) + decltype(z)(
							(float)c * 2 / max_column - 1.5,
							(float)r * 2 / max_row - 1);
		mat[r][c] = (n == max_n ? '#' : '.');
	}


	if(rank == size-1)
		MPI_Send(bloco + (size-1)*piece, max_row*max_column - (size-1)*piece, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
	else
		MPI_Send(bloco + rank*piece, piece, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

	if(rank == 0){
		for(int p = 0; p < size-1; p++){
			MPI_Recv(bloco + p*piece, piece, MPI_CHAR, p, tag, MPI_COMM_WORLD, &status);
		}
		MPI_Recv(bloco + (size-1)*piece, max_row*max_column - (size-1)*piece, MPI_CHAR, size-1, tag, MPI_COMM_WORLD, &status);
		for (int r = 0; r < max_row; ++r)
		{
			for (int c = 0; c < max_column; ++c)
				std::cout << mat[r][c];
			cout << '\n';
		}
	}

	free(bloco);
	free(mat);


	MPI_Finalize();

	return 0;
}


