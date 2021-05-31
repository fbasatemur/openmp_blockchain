#pragma once
#include <string>
#include <iostream>
using namespace std;
typedef unsigned int WORD;

struct Block {	
	size_t blockNum;
	std::string data;
	std::string hash;				// blogun aldigi hash burada
	Block* next;
};

struct BlockChain
{
	Block* head;									// pointer to the head of list
	Block* current;
	size_t counter = 0;
	BlockChain();									
	~BlockChain();										

	bool Empty() const;									// is list empty?
	void AddBack(std::string data, std::string hash);							// add to back of list
	void RemoveFront();									// remove front item of list
	void PrintBlock(int __cdecl tid, size_t& privateNonce);
	string GetText(size_t& privateNonce);
	bool Control(string& hash);
};

BlockChain::BlockChain()							// constructor
{
	head = NULL;
	current = NULL;
}

BlockChain::~BlockChain()							// destructor
{
	while (!Empty()) RemoveFront();
}

bool BlockChain::Empty() const							// is list empty?
{
	return head == NULL;
}

void BlockChain::RemoveFront()							// remove front item
{
	if (Empty())
	{
		std::cout << "List is empty !" << endl;
		return;
	}

	Block* temp = head;									// save current head
	head = head->next;							// skip over old head
	delete temp;												// delete the old head
}

void BlockChain::AddBack(std::string data, std::string hash)
{
	counter++;
	Block* v = new Block;
	v->blockNum = counter;
	v->data = data;
	v->hash = hash;		
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

void BlockChain::PrintBlock(int __cdecl tid, size_t& privateNonce)
{
	cout << "BLOCK: " << current->blockNum << "\t tid: " << tid << endl;
	cout << "initialize hash: " << current->hash << "\t nonce: " << privateNonce << "\t tid: " << tid << endl;
}

string BlockChain::GetText(size_t& privateNonce) {

	return to_string(current->blockNum) + current->data + current->hash + to_string(privateNonce);
}

bool BlockChain::Control(string& hash) {

	for (size_t i = 0; i < current->blockNum; i++)
		if (hash[i] != *"0")
			return false;

	if (hash[current->blockNum] == *"0") return false;

	return true;
}