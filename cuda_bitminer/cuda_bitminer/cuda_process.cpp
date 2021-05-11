#include "cuda_process.h"
#include <assert.h>
#include <cuda_runtime_api.h>
#include <stdlib.h>		
#include <iostream>

void Cuda_Set_Device() {
	// Choose which GPU to run on, change this on a multi-GPU system.
	if (cudaSetDevice(0) != cudaSuccess)
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
}

void Cuda_Malloc(void* gpuP, size_t size, size_t typeSize) {

	cudaError_t cudaStatus = cudaMalloc((void**)gpuP, size * typeSize);
	if (cudaStatus != cudaSuccess)
		fprintf(stderr, "cudaMalloc failed!");
}

void Cuda_Host2Device(void* gpuP, WORD* cpuP, size_t size, size_t typeSize) {
	// Copy input memory from host memory to GPU buffers.
	cudaError_t cudaStatus = cudaMemcpy(gpuP, cpuP, size * typeSize, cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess)
		fprintf(stderr, "cudaMemcpy failed!");

}

void Cuda_Device2Host(WORD* cpuP, void* gpuP, size_t size, size_t typeSize) {
	// Copy output memory from GPU buffer to host memory.
	cudaError_t cudaStatus = cudaMemcpy(cpuP, gpuP, size * typeSize, cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess)
		fprintf(stderr, "cudaMemcpy failed!");

}

void Cuda_Device_Sync() {

	cudaError_t cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess)
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);

}

void Cuda_Free(void* gpuP) {

	assert(cudaFree(gpuP) == cudaSuccess);
}