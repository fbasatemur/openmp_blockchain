#include <stdlib.h>		
#include <iostream>

#include "cuda_process.h"
#include "sha256.cuh"
#include "Block.h"
#include <omp.h>
#define NUM_THREADS 2

using namespace std;

bool Control(string& hash, size_t blockNum) {

	for (size_t i = 0; i < blockNum; i++)
		if (hash[i] != *"0")
			return false;
	
	if (hash[blockNum] == *"0") return false;

	return true;
}


int main()
{

	//const char* text = "Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.";
	string text = "hello world";
	string text1 = "Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.";
	string text2 = "Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir...";

	size_t nonce = 0;
	WORD* k = InitializeKValues();
	WORD* h = InitializeHashValues();


	BlockChain blockChain;
		
	size_t textLen;
	string hash = "0000000000000000000000000000000000000000000000000000000000000000";

	blockChain.addBack(text1, hash);
	cout << "BLOCK: " << blockChain.current->blockNum << endl;
	cout << "initialize hash: " << hash << "\t nonce: " << nonce << endl;
	text1 = blockChain.getText(nonce);		// uretilen text
	while(true)
	{
			
		string binText = PreSHA256(text1, textLen);
		WORD* ha = RecursiveSHA256(binText.c_str(), h, k, textLen, 0);
		hash = WORDToStr(ha, text1);
		delete[] ha;

		if (!Control(hash, blockChain.counter))
		{
			// nonce increment (MUTEX)
			nonce++;
			text1 = blockChain.getText(nonce);
		}
		else {
			cout << "found hash: " << hash << "\t nonce: " << nonce << endl << endl;

			blockChain.addBack(text1, hash);
			nonce = 0;
			cout << "BLOCK: " << blockChain.current->blockNum << endl;
			cout << "initialize hash: "<< hash << "\t nonce: "<< nonce << endl;
			text1 = blockChain.getText(nonce);
		}
	}



	/*Cuda_Set_Device();

	void* gpu1;
	void* gpu2;
	void* gpu3;
	Cuda_Malloc(&gpu1, arraysize, 4);
	Cuda_Malloc(&gpu2, arraysize, 4);
	Cuda_Malloc(&gpu3, arraysize, 4);


	Cuda_Host2Device(gpu1, a, arraysize, 4);
	Cuda_Host2Device(gpu2, b, arraysize, 4);
	Cuda_Host2Device(gpu3, c, arraysize, 4);

	GPUSHA256(gpu3, gpu1, gpu2);

	Cuda_Device2Host(c, gpu3, arraysize, 4);


	cout << "{1,2,3,4,5} + {10,20,30,40,50} = " << c[0] << " " << c[1] << " " << c[2] << " " << c[3] << " " << c[4] << " " << endl;

	Cuda_Device_Sync();

	Cuda_Free(gpu1);
	Cuda_Free(gpu2);
	Cuda_Free(gpu3);*/


	return 0;
}
