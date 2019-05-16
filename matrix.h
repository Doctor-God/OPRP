#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdlib.h>
#include <string.h>

#define random() ((rand() ^ rand()) / (RAND_MAX + 1.0))

typedef struct {
      double **data;
      int rows;
      int cols;
} matrix_t;

typedef struct {
	int id;
	matrix_t *A;
	matrix_t *B;
	matrix_t *C;

    double *vet;
    int vet_size;

    int nthreads;
    pthread_t *threads;

    int** manip_atual;

	int l_i;
	int l_f;
} DadosThread;

typedef struct {
    int id;
    double* vet;
    int left;
    int right;

} thread_info;

/*
 * All functions must return a new matriz (when needed)
 */

matrix_t *matrix_create(int rows, int cols);
matrix_t *matrix_cpy(matrix_t *m);
void matrix_destroy(matrix_t *m);

void matrix_randfill(matrix_t *m);
void matrix_fill(matrix_t *m, double val);

void matrix_print(matrix_t *m);
void array_print(double* vet, int begin, int end);


matrix_t *matrix_multiply_cuda(matrix_t *A, matrix_t *B, matrix_t *C, int n_threads);

int min(int x, int y);
void swap(double* a, double* b);

#endif
