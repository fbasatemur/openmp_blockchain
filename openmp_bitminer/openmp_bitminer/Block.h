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

	bool empty() const;									// is list empty?
	void addBack(std::string data, std::string hash);							// add to back of list
	void removeFront();									// remove front item of list
	void print();
	string getText(size_t nonce);
};

BlockChain::BlockChain()							// constructor
{
	head = NULL;
	current = NULL;
}

BlockChain::~BlockChain()							// destructor
{
	while (!empty()) removeFront();
}

bool BlockChain::empty() const							// is list empty?
{
	return head == NULL;
}

void BlockChain::removeFront()							// remove front item
{
	if (empty())
	{
		std::cout << "List is empty !" << endl;
		return;
	}

	Block* temp = head;									// save current head
	head = head->next;							// skip over old head
	delete temp;												// delete the old head
}

void BlockChain::addBack(std::string data, std::string hash)
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

void BlockChain::print()
{
	if (empty())
	{
		cout << "List is empty !" << endl;
		return;
	}

	Block* first = head;
	while (first != NULL)
	{
		cout << first->blockNum << "\t" << first->hash << endl;
		first = first->next;
	}
}

string BlockChain::getText(size_t nonce) {

	return to_string(current->blockNum) + current->data + current->hash + to_string(nonce);
}

