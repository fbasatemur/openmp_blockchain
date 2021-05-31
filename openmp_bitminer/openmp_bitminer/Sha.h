#include <string>
typedef unsigned int WORD;		// 4 Bytes

std::string Sha256(std::string input, WORD* sha256K);
WORD* InitializeK();
