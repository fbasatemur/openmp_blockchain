#include <stdlib.h>		
#include <iostream>

#include "sha256.h"
//#include "Sha.h"

#include "Block.h"
#include <omp.h>
#include <iomanip>

using namespace std;


int main()
{
	//string inputText = "Bize her yer Trabzon! Bölümün en yakışıklı hocası İbrahim Hoca’dır";		// pdf text
	string inputText = "Bize her yer Trabzon! Bolumun en yakisikli hocasi Ibrahim Hoca'dir";		// ascii
	string initializeHash = "0000000000000000000000000000000000000000000000000000000000000000";

	size_t numThreads;
	cout << "THREAD NUMS: ";
	cin >> numThreads;

	size_t nonce = 0;
	bool flag = false;
	size_t acceptNonce;			// onaylanmasi gereken nonce
	size_t acceptCounter;		// onaylayan thread sayisi

	size_t textLen;
	WORD* k = InitializeKValues();
	WORD* h = InitializeHashValues();
	//WORD* sha256K = InitializeK();
	BlockChain blockChain;
	blockChain.AddBack(inputText, initializeHash);
	cout << "\n#BLOCK: " << blockChain.current->blockNum << "\t nonce: " << nonce << "\t tid: " << omp_get_thread_num() << endl;
	string privateText;
	size_t privateNonce;
	size_t entryCounter;
	omp_lock_t lock;
	omp_init_lock(&lock);
	double start = omp_get_wtime(), end;

#pragma omp parallel shared(nonce,numThreads,textLen,flag,acceptNonce,acceptCounter,blockChain,entryCounter, start,end,h,k) num_threads(numThreads) firstprivate(privateText) private(privateNonce)
	{

#pragma omp critical 
		{
			cout << "initialize hash: " << blockChain.current->hash << "\t nonce: " << nonce << "\t tid: " << omp_get_thread_num() << endl;
			privateNonce = nonce;
			nonce++;
		}
#pragma omp barrier

		while (true)
		{
			privateText = blockChain.GetText(privateNonce);		// uretilen text

			string binText = PreSHA256(privateText, textLen);
			WORD* ha = RecursiveSHA256(binText.c_str(), h, k, textLen, 0);
			string privateHash = WORDToStr(ha, privateText);
			delete[] ha;
			//string privateHash = Sha256(privateText, sha256K);

			if (blockChain.Control(privateHash))
			{
#pragma omp single nowait
				{
					acceptCounter = 0;
					entryCounter = 0;
					acceptNonce = privateNonce;
					flag = true;
					while (acceptCounter < numThreads / 2) {}
					while (entryCounter < numThreads - 1) {}
					blockChain.AddBack(privateText, privateHash);			// onay alindi bir sonraki blogu hazirla
					
					nonce = 0;
					privateNonce = nonce;
					nonce++;

					end = omp_get_wtime();
					cout << privateText << endl << "hash found: " << privateHash << "\t nonce: " << acceptNonce << "\t tid: " << omp_get_thread_num() << "\t time: " << std::setprecision(3) << (end - start) << endl << endl << endl;
					start = omp_get_wtime();

					blockChain.PrintBlock(omp_get_thread_num(), privateNonce);
					flag = false;
				}
			}
			else {

				// Nonce bulunamadi, nonce increment
				omp_set_lock(&lock);
				privateNonce = nonce;
				nonce++;
				omp_unset_lock(&lock);
			}
			
			if (flag) {

				privateText = blockChain.GetText(acceptNonce);

				binText = PreSHA256(privateText, textLen);
				WORD* ha = RecursiveSHA256(binText.c_str(), h, k, textLen, 0);
				privateHash = WORDToStr(ha, privateText);
				delete[] ha;
				//privateHash = Sha256(privateText, sha256K);

#pragma omp critical
				{
					if (acceptCounter < numThreads / 2)			// total de N/2 + kendi = N/2+1 thread onaylamis oldu
					{
						if (blockChain.Control(privateHash)) {
							cout << "hash accepted: " << privateHash << "\t nonce: " << acceptNonce << "\t tid: " << omp_get_thread_num() << endl;
							acceptCounter++;
						}
						else cout << "Error tid: " << omp_get_thread_num() << endl;
					}
					entryCounter++;
				}

				while (flag) {}

				omp_set_lock(&lock);
				privateNonce = nonce;
				nonce++;
				omp_unset_lock(&lock);
			}
		}
	}

	omp_destroy_lock(&lock);
	return 0;
}
