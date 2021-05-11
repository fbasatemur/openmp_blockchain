#pragma once

typedef unsigned int WORD;

void Cuda_Set_Device();
void Cuda_Malloc(void* gpuP, size_t size, size_t typeSize);

void Cuda_Host2Device(void* gpuP, WORD* cpuP, size_t size, size_t typeSize);
void Cuda_Device2Host(WORD* cpuP, void* gpuP, size_t size, size_t typeSize);

void Cuda_Device_Sync();
void Cuda_Free(void* gpuP);