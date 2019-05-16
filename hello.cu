#include <stdio.h>

__device__ const char *str = "Hello World!";
const char str_length = 12;

__global__ void hello(){
    printf("%c\n", str[threadIdx.x % str_length]);
}

int main(void){
    // int num_threads = str_length;
    // int num_blocks = 2;
    // dim3 dimBlock(16, 16);
    // dim3 dimGrid(32, 32);
    hello<<<1,str_length>>>();
    cudaDeviceSynchronize();
    
    return 0;
}