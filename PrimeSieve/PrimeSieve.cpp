// PrimeSieve.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
// Prime.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#include <bitset>
#include <vector>
#include <math.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream> 
//#include <boost/dynamic_bitset.hpp>         
//const size_t max = 1000;
const size_t max = (size_t)4294967295;  //this isn't the size_t max but the maximum array size 0xFFFFFFFF
//const size_t max = (size_t)-1;
//const size_t max = std::numeric_limits<size_t>::max() /10000;
//const size_t max = 1844674407UL;
size_t primeCatlyst()
{
	//int c = 10004
	//var primes = range(3, c, 2); 
	//var ran = range(3, (int)sqrt(c), 2);
	//for (x in ran)
	//primes = filter(function(var y) { return x == y || (y % x != 0); }, primes);
	//print(primes);}
	return 0;
}

void writeBitSetToStream(const char *my_file, std::bitset<max> *bitss)
{
	std::ofstream OutFile;
	//auto my_file = "d:\\binary.bin";
	OutFile.open(my_file, std::ios::out | std::ios::binary);
	OutFile.write((char*)&max, sizeof(size_t));
	char buffer = 0;
	for (size_t i = 0; i < max; i += 8)
	{
		for (char c = 0; c < 8; c++)
		{
			char v = 0;
			if (max <= (i + c))//if we overflow at the end, pack the rest with 0
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
void readBitSetFromStream(const char *my_file, std::bitset<max> *b)
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
size_t nextPrime(std::bitset<max>*bSet, size_t index)
{
	size_t nextP = index;
	bool b = false;
	do {
		nextP = nextP + 1;
		b = bSet->test(nextP);
	} while (b);
	return nextP;
}

std::bitset<max>* primeSieve()
{
	std::cout << "bit set size will be: " << max;
	size_t srt = (size_t)sqrt(max);
	//need to 'new' as stack size is a limiation
	// bitset is all odd numbers
	std::bitset<max> *b = new std::bitset<max>();
	b->set();
	std::cout << std::endl << " allocation worked" << std::endl;
	b->set(0, false); //[0] = 0;  1 is not a prime
	//b->set(1, false);
	size_t index = 3;
	while (index < srt)
	{
		for (size_t i = index * 2; i < max; i += index)
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
	} while (index < max);
	printf("Total primes less than %zu is %zu\n", max, count);
	return b;
}



std::bitset<max>* primeSieve2(std::bitset<max>* bits)
{
	size_t srt = (size_t)sqrt(max);
	std::bitset<max> *b = new std::bitset<max>(); // b(max); // = new std::bitset(max);
	b->set();
	//b->set(0, false); //[0] = 0;
	//b->set(1, false);

	//first bit will be for number size_t+1
	size_t index = 2;
	while (index < srt)
	{
		size_t startIndex = (max % index + 1) % index;  //Figure out the starting index.  ((max +1) % val  == (max % val +1) % val  
		for (size_t i = startIndex; i < max; i += index)
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
	} while (index < max);
	printf("Total primes less than %zu is %zu\n", max, count);
	writeBitSetToStream("d:\\binary2.bin", b);

	//writeBitSetToStream(b);
	//std::bitset<max> *bfile = new std::bitset<max>();
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
	bigint max(4294977295);
	while (num < max)
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

int main(int argc, char*argv[])
{
	auto start = std::chrono::system_clock::now();
	std::bitset<max> *bStart = primeSieve();
	writeBitSetToStream("d:\\binary.bin", bStart);
	std::bitset<max>* bNext = primeSieve2(bStart);
	readBitSetFromStream("d:\\binary2.bin", bNext);


	//bigPrime();
	//WritePrime();
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
#pragma warning(disable : 4996)
	std::cout << "finished computation at " << std::ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";
	std::cout << "end\n";
}
