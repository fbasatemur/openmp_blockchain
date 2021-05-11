#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <bitset>
#include <string>
#include "cuda_process.h"

#ifdef __cplusplus									
extern "C"
#endif // __cplusplus

void GPUSHA256(void* gpu3, void* gpu1, void* gpu2);

WORD* RecursiveSHA256(const char* binText, WORD* h, WORD* k, size_t textLen, size_t step);
std::string PreSHA256(std::string text, size_t& textLen);
WORD* InitializeHashValues();
WORD* InitializeKValues();
std::string WORDToStr(WORD* h);