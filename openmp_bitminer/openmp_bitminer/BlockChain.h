#pragma once
#include <string>
#include <iostream>
using namespace std;
typedef WORD WORD;

struct Block {
	size_t blockNum;
	std::string data;
	std::string initializeHash;							// this hash values start to calculate
	Block* next;
};

struct BlockChain
{
	Block* head;										// pointer to the head of BlockChain
	Block* current;										// is processed current block in BlockChain
	size_t blockCounter = 0;
	BlockChain();
	~BlockChain();
	bool Empty() const;									// is block chain empty?
	void AddBack(std::string data, std::string hash);	// add to back of BlockChain
	void RemoveFront();									// remove front item of BlockChain
	string GetText(size_t& privateNonce);
	bool Control(string& hash);
};

BlockChain::BlockChain()								// constructor
{
	head = NULL;
	current = NULL;
}

BlockChain::~BlockChain()								// destructor
{
	while (!Empty()) RemoveFront();
}

bool BlockChain::Empty() const							
{
	return head == NULL;
}

void BlockChain::RemoveFront()							// remove front item
{
	Block* temp = head;									
	head = head->next;									
	delete temp;										
}

void BlockChain::AddBack(std::string data, std::string hash)		// add new block
{
	blockCounter++;
	Block* v = new Block;
	v->blockNum = blockCounter;
	v->data = data;
	v->initializeHash = hash;
	v->next = NULL;
	current = v;

	if (head == NULL) head = v;
	else
	{
		Block* first = head;
		while (first->next != NULL) first = first->next;
		first->next = v;
	}
}

string BlockChain::GetText(size_t& privateNonce)					// text will forward to SHA256 algorithm
{
	return to_string(current->blockNum) + current->data + current->initializeHash + to_string(privateNonce);
}

bool BlockChain::Control(string& hash)								// condition control
{
	for (size_t i = 0; i < current->blockNum; i++)
		if (hash[i] != *"0")
			return false;

	if (hash[current->blockNum] == *"0") return false;

	return true;
}