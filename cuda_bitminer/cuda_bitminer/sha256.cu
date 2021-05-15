#pragma once
#include <cuda_runtime_api.h>	
#include "device_launch_parameters.h"

#include <sstream>
#include "sha256.cuh"


#define N 512
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define SIGMA0(a) (ROTRIGHT(a,7) ^ ROTRIGHT(a,18) ^ ((a) >> 3))
#define SIGMA1(a) (ROTRIGHT(a,17) ^ ROTRIGHT(a,19) ^ ((a) >> 10))
#define CH(a,b,c) (((a) & (b)) ^ (~(a) & (c)))
#define MAJ(a,b,c) (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))
#define SUM0(a) (ROTRIGHT(a,2) ^ ROTRIGHT(a,13) ^ ROTRIGHT(a,22))
#define SUM1(a) (ROTRIGHT(a,6) ^ ROTRIGHT(a,11) ^ ROTRIGHT(a,25))

using namespace std;


__global__ void addKernel(int* c, int* a, int* b)
{
	int i = threadIdx.x;
	c[i] = a[i] + b[i];
}

void GPUSHA256(void* gpu3, void* gpu1, void* gpu2) {
	addKernel << < 1, 10 >> > ((int*)gpu3, (int*)gpu1, (int*)gpu2);
}

//__global__ void WORK(WORD* src, WORD* result)
//{
//	int idx = blockIdx.x * blockDim.x + threadIdx.x;
//	if (idx > 15) {
//		result[idx] = SIGMA1(src[idx - 2]) + src[idx - 7] + SIGMA0(src[idx - 15]) + src[idx - 16];
//	}
//}




string WORDToStr(WORD* h, string text1) {

	// HEX BASARKEN BASTAKI 0 HEXADECIMAL SAYILARI BASMIYORDU, IF ILE ONARILDI !
	// "Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.000"
	// 0b208c73de9744cf1aab8788d8217f24a0d34b98c6d0e7c17e16e0c4b84db276
	
	string result;
	for (size_t i = 0; i < 8; i++)
	{
		std::stringstream ss;
		ss << hex << h[i];
		for (size_t j = 0; j < 8 - ss.str().length(); j++)
			result += "0";
		result += ss.str();
	}

	// DEBUG ICIN
	if (result.length() != 64) {
		string result;
		for (size_t i = 0; i < 8; i++)
		{
			std::stringstream ss;
			ss << hex << h[i];
			for (size_t j = ss.str().length(); j < 8; j++)
				result += "0";
			result += ss.str();
		}
	}

	return result;
}

//string decToBinary(uint n)
//{
//	uint i = 0, binaryNum[64];
//
//	while (n > 0) {
//		binaryNum[i] = n % 2;
//		n = n / 2;
//		i++;
//	}
//
//	string result;
//	for (int step = i - 1; step >= 0; step--) {
//		result += to_string(binaryNum[step]);
//	}
//	return result;
//}
//
//void decToHexa(int num)
//{
//	// char array to store hexadecimal number
//	char hexaDeciNum[64];
//	int i = 0, temp = 0;
//
//	while (num != 0) {
//
//		temp = num % 16;
//
//		if (temp < 10) hexaDeciNum[i] = temp + 48;
//		else hexaDeciNum[i] = temp + 55;
//
//		num = num / 16;
//		i++;
//	}
//
//	// hexadecimal number array in reverse order
//	char temp1;
//	int j = 0;
//	for (int step = i - 1; step >= 0; step--) {
//		temp1 = hexaDeciNum[step];
//		hexaDeciNum[step] = hexaDeciNum[j];
//		hexaDeciNum[j] = temp1;
//		j++;
//	}
//}


WORD* InitializeHashValues() {

	WORD* h = new WORD[8];
	h[0] = 0x6a09e667;
	h[1] = 0xbb67ae85;
	h[2] = 0x3c6ef372;
	h[3] = 0xa54ff53a;
	h[4] = 0x510e527f;
	h[5] = 0x9b05688c;
	h[6] = 0x1f83d9ab;
	h[7] = 0x5be0cd19;
	return h;
}

WORD* InitializeKValues() {

	WORD* k = new WORD[64]{
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2 };

	return k;
}


void WordExtend(WORD* words) {

	// seri olmak zorunda
	for (size_t i = 16; i < 64; i++)
		words[i] = SIGMA1(words[i - 2]) + words[i - 7] + SIGMA0(words[i - 15]) + words[i - 16];
}

void WordCompress(WORD* abcdefgh, WORD* k, WORD* expandedWords) {

	// seri olmak zorunda, YALNIZCA FOR ICI PARALEL YAPILABILIR, 64 TH OLMAZ
	WORD temp1, temp2;
	for (size_t i = 0; i < 64; ++i) {
		temp1 = abcdefgh[7] + SUM1(abcdefgh[4]) + CH(abcdefgh[4], abcdefgh[5], abcdefgh[6]) + k[i] + expandedWords[i];
		temp2 = SUM0(abcdefgh[0]) + MAJ(abcdefgh[0], abcdefgh[1], abcdefgh[2]);
		abcdefgh[7] = abcdefgh[6];
		abcdefgh[6] = abcdefgh[5];
		abcdefgh[5] = abcdefgh[4];
		abcdefgh[4] = abcdefgh[3] + temp1;
		abcdefgh[3] = abcdefgh[2];
		abcdefgh[2] = abcdefgh[1];
		abcdefgh[1] = abcdefgh[0];
		abcdefgh[0] = temp1 + temp2;
	}
}

WORD* SHA256(WORD* words, WORD* h, WORD* k) {

	WORD* expandedWord = new WORD[64]();
	memcpy(expandedWord, words, 16 * sizeof(WORD));
	// kelime genisletme algoritmasi
	WordExtend(expandedWord);

	// initialize hash values copy to abcdefgh
	WORD* abcdefgh = new WORD[8];	// 8 * 4 = 32 Byte => 256 bits
	memcpy(abcdefgh, h, 8 * sizeof(WORD));

	// word compress algoritmasi
	WordCompress(abcdefgh, k, expandedWord);

	abcdefgh[0] = h[0] + abcdefgh[0];
	abcdefgh[1] = h[1] + abcdefgh[1];
	abcdefgh[2] = h[2] + abcdefgh[2];
	abcdefgh[3] = h[3] + abcdefgh[3];
	abcdefgh[4] = h[4] + abcdefgh[4];
	abcdefgh[5] = h[5] + abcdefgh[5];
	abcdefgh[6] = h[6] + abcdefgh[6];
	abcdefgh[7] = h[7] + abcdefgh[7];

	return abcdefgh;
}

WORD* BinTextToWORD(const char* text) {

	// tum degerler 0 ile initialize edilir
	WORD* words = new WORD[64]();

	// 16 TH ILE PARALLEL 
	// gelen blokta her biri, bir biti temsil eden 16 * 32 => 512 karakter var
	for (size_t i = 0; i < 16; i++)
	{
		for (size_t j = 0; j < 32; j++)
		{
			words[i] = ROTLEFT(words[i], 1);
			if (text[i * 32 + j] == *"1")
				words[i] = words[i] | (WORD)1;
		}
	}

	return words;
}

// PARALLEL
string TextToBinaryStr(string words)
{
	string binaryString = "";
	for (char& _char : words) {
		binaryString += bitset<8>(_char).to_string();
	}
	return binaryString;
}

// PARALLEL
string PreSHA256(string text, size_t& textLen) {

	string binText = TextToBinaryStr(text);
	size_t binTextLen = binText.length();							// text in binary halinin uzunlugu

	string textLenBin = bitset<64>(text.length() * 8).to_string();	// text uzunlugunun binary degeri => 64 bits uzunlugunda

	int padding = N - (binTextLen + textLenBin.length()) % N;

	binText += "1";
	for (int i = 1; i < padding; i++)
		binText += "0";

	binText += textLenBin;
	textLen = binText.length();

	return binText;
}

WORD* RecursiveSHA256(const char* binText, WORD* h, WORD* k, size_t textLen, size_t step) {

	if (step == textLen)
		return h;

	WORD* wordBlock = BinTextToWORD(&binText[step]);
	WORD* hash = SHA256(wordBlock, h, k);
	RecursiveSHA256(binText, hash, k, textLen, step + N);
}