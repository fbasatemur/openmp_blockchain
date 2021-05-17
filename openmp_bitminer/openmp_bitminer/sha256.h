#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <bitset>
#include <string>
typedef unsigned int WORD;

WORD* RecursiveSHA256(const char* binText, WORD* h, WORD* k, size_t textLen, size_t step);
std::string PreSHA256(std::string& text, size_t& textLen);
WORD* InitializeHashValues();
WORD* InitializeKValues();
std::string WORDToStr(WORD* h, std::string text1);