#include <stdlib.h>		
#include <iostream>

#include "sha256.h"
#include "Block.h"
#include <omp.h>

using namespace std;

bool Control(string& hash, size_t& blockNum) {

	for (size_t i = 0; i < blockNum; i++)
		if (hash[i] != *"0")
			return false;

	if (hash[blockNum] == *"0") return false;

	return true;
}


int main()
{

	string inputText = "Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir.";

	size_t nonce = 0;
	WORD* k = InitializeKValues();
	WORD* h = InitializeHashValues();
	size_t textLen;
	string initializeHash = "0000000000000000000000000000000000000000000000000000000000000000";

	size_t numThreads;
	cout << "THREAD NUMS: ";
	cin >> numThreads;

	bool flag1 = false;
	bool flag2 = false;
	size_t acceptNonce;			// onaylanmasi gereken nonce
	size_t acceptCounter = 0;	// onaylayan thread sayisi

	BlockChain blockChain;
	blockChain.addBack(inputText, initializeHash);
	cout << "\nBLOCK: " << blockChain.current->blockNum << endl;
	string privateText;
	size_t privateNonce;
	size_t entryCounter = 0;

#pragma omp parallel shared(nonce,h,k,numThreads,flag1,flag2,acceptNonce,acceptCounter,blockChain,entryCounter) num_threads(numThreads) firstprivate(privateText) private(privateNonce, textLen)
	{

#pragma omp critical 
		{
			cout << "initialize hash: " << blockChain.current->hash << "\t nonce: " << nonce << "\t tid: " << omp_get_thread_num() << endl;
			privateText = blockChain.getText(nonce);		// uretilen text
			privateNonce = nonce;
			nonce++;
		}

		while (true)
		{
			string binText = PreSHA256(privateText, textLen);
			WORD* ha = RecursiveSHA256(binText.c_str(), h, k, textLen, 0);
			string hash = WORDToStr(ha, privateText);
			delete[] ha;

			if (!Control(hash, blockChain.counter))
			{
				// Nonce bulunamadi, nonce increment
#pragma omp critical
				{
					nonce++;
					privateNonce = nonce;
					privateText = blockChain.getText(nonce);
				}
			}
			else {
#pragma omp single nowait
				{
					// Nonce bulundu, nonce onaylatmalisin
					acceptNonce = privateNonce;		// bu thread in buldugu nonce onaylatilmali
					flag1 = true;
					while (entryCounter < (numThreads - 1)) {}		// diger threadlerin girmesini bekle
					acceptCounter = 0;

					cout << endl << "found hash: " << hash << "\t nonce: " << acceptNonce << "\t tid: " << omp_get_thread_num() << endl << endl << endl;
					blockChain.addBack(privateText, hash);

					nonce = 0;
					privateNonce = nonce;
					nonce++;
					flag1 = false;

					cout << "BLOCK: " << blockChain.current->blockNum << "\t tid: " << omp_get_thread_num() << endl;
					cout << "initialize hash: " << hash << "\t nonce: " << privateNonce << "\t tid: " << omp_get_thread_num() << endl;
					privateText = blockChain.getText(privateNonce);
					flag2 = true;
					while (flag2) {}		// tum threadlerin cikmasini bekle
				}
			}

			if (flag1 == true) {

#pragma omp critical
				{
					if (acceptCounter < numThreads / 2)			// total de N/2 + kendi = N/2+1 thread onaylamis oldu
					{
						privateText = blockChain.getText(acceptNonce);
						binText = PreSHA256(privateText, textLen);
						WORD* ha = RecursiveSHA256(binText.c_str(), h, k, textLen, 0);
						hash = WORDToStr(ha, privateText);
						delete[] ha;

						if (Control(hash, blockChain.counter)) {

							cout << "accepted hash: " << hash << "\t nonce: " << acceptNonce << "\t tid: " << omp_get_thread_num() << endl;

						}
						else cout << "HATA tid: " << omp_get_thread_num() << endl;

						acceptCounter++;
					}
					entryCounter++;
				}

				while (!flag2) {}

#pragma omp critical 
				{
					privateText = blockChain.getText(nonce);		// uretilen text
					privateNonce = nonce;
					nonce++;
					entryCounter--;
					if (entryCounter == 0) flag2 = false;
				}
			}
		}
	}

	return 0;
}
