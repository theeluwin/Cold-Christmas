//
//  main.cpp
//
//  Created by Team "Cold Christmas" on 12. 2. 24..
//  Copyright 2012 PoolC. All rights reserved.
//

#include <fstream>
#include "cold_christmas.cpp"

#define CC cold_christmas

std::vector<int> get_prime();
std::vector<int> get_prime()
{
	std::ifstream fin("primes.txt");
	std::vector<int> ret;
	int p;
	while(fin >> p) { ret.push_back(p); }
	return ret;
}
std::vector<int> CC::primes = get_prime();

int main(int argc, char *argv[])
{
	std::cout << CC::print(CC::divisor(16)) << std::endl;
	std::cout << CC::print(CC::primitive_root(121)) << std::endl;
	return 0;
}