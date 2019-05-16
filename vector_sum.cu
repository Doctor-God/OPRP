#define N 1200
#define THREADS 1024
#include <stdio.h>
#include <math.h>

__global__ void vecAdd(int *a, int *b, int *c);

int main(){
    int *a, *b, *c;
    int *dev_a, *dev_b, *dev_c;
    int size;

    size = N*sizeof(int);

    cudaMalloc((void**) &dev_a, size);
    cudaMalloc((void**) &dev_b, size);
    cudaMalloc((void**) &dev_c, size);

    a = (int *)malloc(size);
    b = (int *)malloc(size);
    c = (int *)malloc(size);

    for(int i = 0; i < N; i++){
        a[i] = b[i] = i;
        c[i] = 0;
    }

    cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, size, cudaMemcpyHostToDevice);

    vecAdd<<<(int)ceil(THREADS/N),N>>>(dev_a, dev_b, dev_c);

    cudaMemcpy(c, dev_c, size, cudaMemcpyDeviceToHost);

    for(int i = 0; i < N; i++){
        printf("c[%d] = %d\n", i, c[i]);
    }

    free(a);
    free(b);
    free(c);
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);
    
    exit(0);
}

__global__ void vecAdd(int *a, int *b, int *c){
    int i = blockIdx.x*blockDim.x + threadIdx.x;
    if(i < N){
        c[i] = a[i] + b[i];
        printf("Sou a thread %d em %d\n", threadIdx.x, i);
    }
}