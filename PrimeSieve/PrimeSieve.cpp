// PrimeSieve.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <bitset>
#include <vector>
#include <math.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream> 

#include "PrimeSieve.h"

PrimeSieve::PrimeSieve()
{
}


PrimeSieve::~PrimeSieve()
{
}

void PrimeSieve::ReadBitSetFromStream(std::bitset<MaxArraySize>* b, std::istream stream)
{
}

void PrimeSieve::WriteBitSetToStream(std::bitset<MaxArraySize>* b, std::ostream stream)
{
}

void PrimeSieve::ReadBitSetFromFile(std::bitset<MaxArraySize>* b, std::string fileName)
{
}

void PrimeSieve::WriteBitsetToFile(std::bitset<MaxArraySize>* b, std::string fileName)
{
}

void PrimeSieve::SievePrimes(std::bitset<MaxArraySize>* b)
{
}

void PrimeSieve::SievePrimes(std::bitset<MaxArraySize>* b, size_t blockIndex)
{
}


void writeBitSetToStream(const char *my_file, std::bitset<MaxArraySize> *bitss)
{
	std::ofstream OutFile;
	OutFile.open(my_file, std::ios::out | std::ios::binary);
	OutFile.write((char*)&MaxArraySize, sizeof(size_t));
	char buffer = 0;
	for (size_t i = 0; i < MaxArraySize; i += 8)
	{
		for (char c = 0; c < 8; c++)
		{
			char v = 0;
			if (MaxArraySize <= (i + c))//if we overflow at the end, pack the rest with 0
			{
				v = 0;
			}
			else
			{
				v = bitss->test(i + c) << c;
				//printf("%d", (bitss->test(i + c)) ? 1 : 0);
			}
			buffer = buffer + v; //set one bit at a time
		}
		OutFile.write(&buffer, 1);
		buffer = 0;
	}
	OutFile.close();
	printf("\n");
}
void readBitSetFromStream(const char *my_file, std::bitset<MaxArraySize> *b)
{
	std::ifstream InFile;
	//auto my_file = "d:\\binary.bin";
	InFile.open(my_file, std::ios::out | std::ios::binary);
	size_t count = 0;
	InFile.read((char*)&count, sizeof(size_t));
	//printf("there are %d bitss in this file", count);
	char buffer = 0;
	for (size_t i = 0; i <= count / 8; i++)
	{
		InFile.read(&buffer, 1);
		for (char c = 0; c < 8; c++)
		{
			if ((i * 8 + c) < count)
			{
				char bit = 1;
				if (buffer & (bit << c))
				{
					b->set(i * 8 + c);
					//printf("%d", 1);
				}
				else
				{
					b->set(i * 8 + c, false);
					//printf("%d", 0);
				}
			}
			else
			{
				//we are in the last byte in the file with buffer bitss to be ignored.
			}


		}
	}
	InFile.close();
}

//Gets the next prime number in a bitset given the previous primenumber
size_t nextPrime(std::bitset<MaxArraySize>*bSet, size_t index)
{
	size_t nextP = index;
	bool b = false;
	do {
		nextP = nextP + 1;
		b = bSet->test(nextP);
	} while (b);
	return nextP;
}

std::bitset<MaxArraySize>* primeSieve()
{
	std::cout << "bit set size will be: " << MaxArraySize;
	size_t srt = (size_t)sqrt(MaxArraySize);
	//need to 'new' as stack size is a limiation
	// bitset is all odd numbers
	std::bitset<MaxArraySize> *b = new std::bitset<MaxArraySize>();
	b->set();
	std::cout << std::endl << " allocation worked" << std::endl;
	b->set(0, false); //[0] = 0;  1 is not a prime
	//b->set(1, false);
	size_t index = 3;
	while (index < srt)
	{
		for (size_t i = index * 2; i < MaxArraySize; i += index)
		{
			b->set(i, 0);
		}
		do
		{
			index++;
		} while (b->test(index) == 0);
	}

	//all done.  Now lets count how many we have
	size_t count = 1; //we start with 1 because 2 is not counted in this list but it is prime
	index = 0;
	do {
		if (b->test(index) != 0)
		{
			count = count + 1;
		}
		index++;
	} while (index < MaxArraySize);
	printf("Total primes less than %zu is %zu\n", MaxArraySize, count);
	return b;
}



std::bitset<MaxArraySize>* primeSieve2(std::bitset<MaxArraySize>* bits)
{
	size_t srt = (size_t)sqrt(MaxArraySize);
	std::bitset<MaxArraySize> *b = new std::bitset<MaxArraySize>(); // b(MaxArraySize); // = new std::bitset(MaxArraySize);
	b->set();
	//b->set(0, false); //[0] = 0;
	//b->set(1, false);

	//first bit will be for number size_t+1
	size_t index = 2;
	while (index < srt)
	{
		size_t startIndex = (MaxArraySize % index + 1) % index;  //Figure out the starting index.  ((MaxArraySize +1) % val  == (MaxArraySize % val +1) % val  
		for (size_t i = startIndex; i < MaxArraySize; i += index)
		{
			b->set(i, 0);
		}
		do
		{
			index++;
		} while (b->test(index) == 0);
	}
	size_t count = 0;
	index = 0;
	do {
		if (b->test(index) != 0)
		{
			//printf("%d\n", index);
			count = count + 1;
			index++;
		}
	} while (index < MaxArraySize);
	printf("Total primes less than %zu is %zu\n", MaxArraySize, count);
	writeBitSetToStream("d:\\binary2.bin", b);

	//writeBitSetToStream(b);
	//std::bitset<MaxArraySize> *bfile = new std::bitset<MaxArraySize>();
	//readBitSetFromStream(bfile);
	return 0;
}

int WritePrime()
{
	std::ifstream InFile;
	auto in_file = "d:\\binary.bin";
	InFile.open(in_file, std::ios::out | std::ios::binary);
	std::ifstream InFile2;
	auto in_file2 = "d:\\binary2.bin";
	InFile2.open(in_file2, std::ios::out | std::ios::binary);

	std::ofstream OutFile;
	auto out_file = "d:\\primes2.txt";
	OutFile.open(out_file, std::ios::out);
	std::ofstream OutFile2;
	auto out_file2 = "d:\\primesData.txt";
	OutFile2.open(out_file2, std::ios::out);

	size_t count = 0;
	InFile.read((char*)&count, sizeof(size_t));
	//printf("there are %d bitss in this file", count);
	char buffer = 0;
	size_t primeCount = 0;
	for (size_t i = 0; i <= count / 8; i++)
	{
		InFile.read(&buffer, 1);
		for (char c = 0; c < 8; c++)
		{
			size_t val = i * 8 + c;
			if (val % 10000000 == 0)
			{
				OutFile2 << primeCount << std::endl;
				std::cout << primeCount << std::endl;
				primeCount = 0;
			}
			if (val < count)
			{
				char bit = 1;

				if (buffer & (bit << c))
				{
					//std::cout << val << std::endl;
					OutFile << val << std::endl;
					primeCount++;

				}
				else
				{
					//b->set(i * 8 + c, false);
					//printf("%d", 0);
				}
			}
			else
			{
				//we are in the last byte in the file with buffer bitss to be ignored.
			}
		}

	}
	InFile.close();
	OutFile.close();
	return 0;
}

#include "catalyst.hpp"
#include "PrimeSieve.h"
int bigPrime()
{
	std::ofstream OutFile;
	auto my_file = "d:\\primes.txt";
	OutFile.open(my_file, std::ios::out);

	//OutFile << 2 << std::endl;
	//OutFile << 3 << std::endl;
	bigint num(5);
	bool prime = true;
	bigint count(3);
	bigint MaxArraySize(4294977295);
	while (num < MaxArraySize)
	{
		do
		{
			if (num%count == 0)
			{
				prime = false;
			}
			else
			{
				count += 2;
			}
		} while (prime && (count < num));
		if (prime)
		{
			std::cout << num << std::endl;
			OutFile << num << std::endl;
		}
		prime = true;
		num = num + 2;
		count = 3;
	}
	OutFile.close();

	return 0;



}


