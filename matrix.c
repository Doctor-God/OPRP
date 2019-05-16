// gcc -pg -Wall main.c matrix.c -o main
// ./main 200 200
// gprof main gmon.out

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "matrix.h"

matrix_t *matrix_create(int rows, int cols){
    matrix_t *m = NULL;
    unsigned int i;

    m = (matrix_t*) malloc( sizeof(matrix_t) );
    m->rows = rows;
    m->cols = cols;

    m->data = (double**) malloc(sizeof(double*) * rows);
    double* mat = (double*) malloc(sizeof(double) * rows * cols);

    for(i=0; i < rows; i++){
        m->data[i] = &mat[i*cols];
    }

    matrix_fill(m, 0.0);

    return m;
}

matrix_t *matrix_cpy(matrix_t *m){
    int i;
    matrix_t *C = matrix_create(m->rows, m->cols);

    memcpy(C->data[0], m->data[0], sizeof(double) * m->rows * m->cols);
    for(i=0; i < C->rows; i++){
        C->data[i] = &C->data[0][i*C->cols];
    }

    return C;
}

void matrix_destroy(matrix_t *m){
    free(m->data[0]);
    free(m->data);
    free(m);

    return;
}

void matrix_randfill(matrix_t *m){
    int i, j;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            m->data[i][j] = random();
        }
    }
}

void matrix_fill(matrix_t *m, double val){
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         m->data[i][j] = val;
      }
   }
}

void matrix_print(matrix_t *m){

   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         printf("%.2f ", m->data[i][j]);
      }
      printf("\n");
   }
   fflush(stdout);
}

void array_print(double* vet, int begin, int end){
    int i;
    for(i = begin; i <= end; i++){
        printf("%.2f ", vet[i]);
    }
    printf("\n");
}


matrix_t *matrix_multiply_cuda(matrix_t *A, matrix_t *B, matrix_t *C, int n_threads){
    //Não podemos multiplicar
    if(A->cols != B->rows){
        printf("Impossível multiplicar\n");
        printf("m1: %d | m2: %d\n", A->cols, B->rows);
        return NULL;
    }

    double** espaco_threads = (double**) malloc(sizeof(double*)*n_threads);

    int size = (A->n_rows*B->n_cols)/n_threads;
    int tam_vet = A->n_rows*B->n_cols;

    for(int i = 0; i < n_threads-1; i++){
        cudaMalloc((void**) &espaco_threads[i], size*sizeof(double));
        cudMemcpy(espaco_threads[i], size*i, size, cudaMemcpyHostToDevice);
    }
    cudaMalloc((void**) &espaco_threads[i], A->n_rows*B->n_cols-(size*(n_threads-2))*sizeof(double));
    cudMemcpy(espaco_threads[i], size*(n_threads-2)+1, A->n_rows*B->n_cols-(size*(n_threads-2)), cudaMemcpyHostToDevice);

    int i,j,k;
    for(i = 0; i < A->rows; i++){
        for(j = 0; j < B->cols; j++){
            for(k = 0; k < A->cols; k++){
                C->data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }

    return C;
}

