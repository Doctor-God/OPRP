#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "matrix.h"


double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}


int main(int argc, char **argv){
    double start_time, end_time;
    int nrows, ncols, nthreads, exec;

    double* teste = (double*) malloc(sizeof(double) * 10);
    teste[0] = 7;
    teste[1] = 0;
    teste[2] = 8;
    teste[3] = 3;
    teste[4] = 9;
    teste[5] = 4;
    teste[6] = 6;
    teste[7] = 2;
    teste[8] = 5;
    teste[9] = 1;


    matrix_t *m1, *m2, *m3;

    if ((argc != 5)) {
        printf("Uso: %s <rows> <cols> <numero de threads> <tipo de execucao>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    nrows = atoi(argv[1]);
    ncols = atoi(argv[2]);
    nthreads = atoi(argv[3]);
    exec = atoi(argv[4]);

    m1 = matrix_create(nrows, ncols);
    matrix_randfill(m1);
    // printf("\nm1:\n"); matrix_print(m1);

    m2 = matrix_create(nrows, ncols);
    matrix_randfill(m2);
    // printf("\nm2:\n"); matrix_print(m2);

    m3 = matrix_create(m1->rows, m2->cols);
    // printf("\nm3:\n"); matrix_print(m3);

    start_time = wtime();

    m3 = matrix_multiply_serial(m1, m2, m3);

    end_time = wtime();


    matrix_destroy(m1);
    matrix_destroy(m2);
    matrix_destroy(m3);

    printf("%f\n",end_time - start_time);
    fflush(stdout);

    return EXIT_SUCCESS;
}
