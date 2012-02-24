//
//  cold_christmas.cpp
//
//  Created by Team "Cold Christmas" on 12. 2. 24..
//  Copyright 2012 PoolC. All rights reserved.
//

//#pragma once

// Streams
#include <iostream>
#include <sstream>

// STL
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <algorithm>
#include <cmath>

// Namespace
using std::list;
using std::string;


// CC
class cold_christmas
{
    
public:
    
	// Constructor
	cold_christmas() {}
    
	// Initializer
	static void init(std::vector<int> _primes)
	{
		primes = _primes;
	}
    
	// Variables
	static std::vector<int> primes;
    
	// Struct Prototypes
    struct prime_factor
	{	
		int radix;
		int exp;
		prime_factor(int r, int e) : radix(r), exp(e) {}
	};
	struct linear_equation
	{
		int a;
		int x;
		int b;
		int y;
		int c;
        
		linear_equation(int a = 0, int x = 0, int b = 0, int y = 0, int c = 0) 
        : a(a), x(x), b(b), y(y), c(c) {}
        
		linear_equation(string eq);
        
		linear_equation operator+(linear_equation e)
		{
			return linear_equation(e.c, this->y, this->a, this->x - e.x * this->y, this->c);
		}
        
		string print()
		{
			std::stringstream ret;
			ret << a << "*" <<  x << " + " << b << "*" <<  y << " = " << c << std::endl;
			return ret.str();
		}
	};
	struct congruent_equation
	{
		int a;
		int b;
		int n;
		congruent_equation(int a, int b, int n) : a(a), b(b), n(n) {}
		congruent_equation(string eq);
	};
    
	// Normal Modules
	template <typename T>
	static string print(list<T> _list)
	{
		std::stringstream ret;
		for(list<int>::iterator it = _list.begin(); it != _list.end(); it++)
			ret << *it << std::endl;
		return ret.str();
	}
	static bool is_num(char c)
	{
		return (c >= '0' && c <= '9');
	}
	static int string_to_int(string value)
	{
		std::stringstream sstr(value);
		int temp;
		sstr >> temp;
		return temp;
	}
    
	// Math Modules
	static bool is_prime(int n)
	{
		int plen = (int)primes.size();
		for(int i = 0; i < plen; i++) if(primes[i] == n) return true;
		return false;
	}
	static int gcd(int n, int m)
	{
		if(n == 0 || m == 0) return n? n : m;
		else return n < m? gcd(n, m % n) : gcd(n % m, m); 
	}
	static int lcm(int n, int m)
	{
		return  n / gcd(n, m) * m;
	}
	static bool relative_prime(int n, int m)
	{
		return gcd(n, m) == 1;
	}
	static list<int> relative_prime(int n)
	{
		list<int> ret;
        
		if(n == 2) 
		{
			ret.push_back(1);
			return ret;
		}
        
		for(int i = 1; i <= n / 2; i++)
		{
			if(relative_prime(i, n)) 
			{
				ret.push_back(i);
				ret.push_back(n - i);
			}
		}
        
		ret.sort();
		return ret;
	}
	static list<prime_factor> prime_factorization (int n)
	{
		list<prime_factor> ret;
		for(std::vector<int>::iterator it = primes.begin(); n > 1; it++)
		{
			if(n % *it == 0) 
			{
				ret.push_back(prime_factor(*it, 0));
				while(n % *it == 0)
				{
					n /= *it;
					ret.back().exp++;
				}
			}
		}
		return ret;
	}
	static list<int> divisor(int n)
	{
		list<prime_factor> pf = prime_factorization(n);
		list<int> ret;
		ret.push_back(1);
        
		for(list<prime_factor>::iterator div = pf.begin(); div != pf.end(); div++)
		{
			list<int> temp = ret;
            
			for(list<int>::iterator it = temp.begin(); it != temp.end(); it++)
			{
				for(int i = div->exp; i; i--)
				{
					ret.push_back((int)std::pow((double)div->radix, i) * *it);
				}
			}
		}
		ret.sort();
		return ret;
	}
	static int phi(int n)
	{ 
		return (int)relative_prime(n).size();
	}
	static int inverse(int a, int n)
	{
		if(a == 1) return 1;
        
		//phase 0 : Determine if the congruence has a solution.
		if(gcd(a, n) > 1)
		{
			std::cout << "has no inverse" << std::endl;
			return 0;
		}
        
		//Get the solution by using Euclidean algorithm.
        
		//phase 1 : push linear equations while the remainder is nonzero
		std::stack<linear_equation> stack;
		stack.push(linear_equation(a, n / a, n % a, 1, n));
		while(stack.top().b)
		{
			stack.push(linear_equation(stack.top().b, stack.top().a / stack.top().b, stack.top().a % stack.top().b, 1, stack.top().a));
		}
        
		//phase 2 : pop pop baby
		stack.pop();
		linear_equation eq = linear_equation(stack.top().c, 1, stack.top().a, -stack.top().x, 1);
		stack.pop();
		while(stack.size())
		{
			eq = eq + stack.top();
			stack.pop();
		}
        
		return ((eq.a == a? eq.x : eq.y) % n + n) % n ;
	}
	static int mod_pow(int r, int p, int n)
	{
		int ret = 1;
		for(int i = 0; i < p; i++) ret = (ret * r) % n;
		return ret;
	}
	
	// Primitive Root
	static list<int> primitive_root (int n)
	{
		//phase 0 : determine if n has a primitive root
		// only 2, 4, p^k, or 2 * p^k have primitive roots
		list<prime_factor> pf = prime_factorization(n);
		list<int> ret;
        
		if(n == 2 || n == 4 ||
           (pf.size() == 1 && pf.front().radix != 2) ||
           (pf.size() == 2 && pf.front().radix == 2 && pf.front().exp == 1))
		{
			//phase 1 : find the smallest primitive root of n, r
			int r;
			list<int> divisors_phi_n = divisor(phi(n));
			divisors_phi_n.pop_back();
			list<int> relative_prime_n = relative_prime(n);
            
			for(list<int>::iterator it = relative_prime_n.begin(); it != relative_prime_n.end(); it++)
			{
				bool flag = false;
				for(list<int>::iterator it2 = divisors_phi_n.begin(); it2 != divisors_phi_n.end(); it2++)
				{
					if(mod_pow(*it, *it2, n) == 1)
						flag = true;
				}
				r = *it;
				if(!flag) break;
			}
            
			//phase 2 : return the list of the r^p where p in relativePrime(phi(n))
			list<int> exp = relative_prime(phi(n));
			for(list<int>::iterator it = exp.begin(); it != exp.end(); it++)
			{
				ret.push_back(mod_pow(r, *it, n));
			}
		}
		else
		{
			std::cout << "the integer has no primitive root" << std::endl;
		}
        
		ret.sort();
		return ret;
	}
    
	// Chinease Remainder Thm
	static congruent_equation string_to_equation(string input)
	{
		int a, b, n;
		string temp = "";
		int i = 0;
        
		while(!is_num(input[i])) i++;
		while(is_num(input[i]))	temp += input[i++];
        
		a = string_to_int(temp);
		temp = "";
        
		while(!is_num(input[i])) i++;
		while(is_num(input[i])) temp += input[i++];
        
		b = string_to_int(temp);
		temp = "";
        
		while(!is_num(input[i])) i++;
		while(is_num(input[i]))	temp += input[i++];
        
		n = string_to_int(temp);
        
		return congruent_equation(a, b, n);
	}
	static list<int> linear_congruence(string eq)
	{
		return linear_congruence(string_to_equation(eq));
	}
	static list<int> linear_congruence(int a, int b, int n)
	{
		return linear_congruence(congruent_equation(a, b, n));
	}
	static list<int> linear_congruence(int a, int b, int c, int n)
	{
		return linear_congruence(a, c - b, n);
	}
	static list<int> linear_congruence(congruent_equation eq)
	{
		list<int> ret;
		int a = eq.a, b = eq.b, n = eq.n;
        
		//phase 0 : Determine if the congruence has a solution.
		int gcd_ = gcd(a, n);
		if(b % gcd_)
		{
			std::cout << "the congruence has no solution" << std::endl;
		}
		else
		{
			//phase 1 : Determine if (a, n) = 1. If not, divide a and n by (a, n).
			if(gcd_ > 1)
			{
				a /= gcd_;
				b /= gcd_;
				n /= gcd_;
			}
            
			//phase 2 : Find the special solution of ax = 1 (mod n) by solving ax - ny = 1.
			a = a % n;
			int sol = (inverse(a, n) * b) % n;
            
			//phase 3 : return the all solutions
			for(int i = gcd_ - 1; i >= 0; i--)
			{
				ret.push_back(sol + i * n);
			}
		}
        
		ret.sort();
		return ret;
	}
	//static int system_of_congruence(string eqs);
	static int system_of_congruence(list<congruent_equation> eqs)
	{
		int N = 1; 
        
		//phase 1 : if two congruences has gcd > 1, merge them
		for(list<congruent_equation>::iterator it = eqs.begin(); it != eqs.end(); it++)
		{
			list<congruent_equation>::iterator it2 = it;
			it2++;
			while(it2 != eqs.end())
			{
				if(gcd(it->n, it2->n) != 1)
				{
					int M = lcm(it->n, it2->n);
                    
					list<int> sol1 = linear_congruence(it->a, it->b, it->n); // 2, 5
					list<int> sol2 = linear_congruence(it2->a, it2->b, it2->n); // 2
					list<int> temp;
                    
					for(int i = 1; i < M / it->n; i++) // i : 1 to 3-1=2
					{
						for(list<int>::iterator j = sol1.begin(); j != sol1.end(); j++)
							temp.push_back(*j + it->n * i); //(2, 5) (8, 11) (14 17)
					}
					sol1.splice(sol1.end(), temp);
					temp.clear();
                    
					for(int i = 1; i < N / it2->n; i++)
					{
						for(list<int>::iterator j = sol2.begin(); j != sol2.end(); j++)
							temp.push_back(*j + it2->n * i);
					}
					sol2.splice(sol2.end(), temp);
                    
					bool flag = true;
					for(list<int>::iterator i = sol1.begin(); i != sol1.end() && flag; i++)
					{
						for(list<int>::iterator j = sol2.begin(); j != sol2.end() && flag; j++)
						{
							if(*i == *j)
							{
								*it = congruent_equation(1, *i, N);
								list<congruent_equation>::iterator t = it2;
								it2++;
								eqs.erase(t);
								flag = false;
							}
						}
					}
                    
					if(flag)
					{
						std::cout << "the congruences have no solution" << std::endl;
						return 0;
					}
				}
				it2++;
			}
		}
        
		//phase 2 : set the N
		for(list<congruent_equation>::iterator it = eqs.begin(); it != eqs.end(); it++)
		{
			N *= it->n;
		}
        
        
		//phase 3 : now every n is relative prime each other
		//			return the solution
		int ret = 0;
        
		for(list<congruent_equation>::iterator it = eqs.begin(); it != eqs.end(); it++)
		{
			ret += linear_congruence(N / it->n, it->b, it->n).front() * N / it->n; 	
		}
        
		return ret % N;
	}
    
};


/*
 int cold_christmas::system_of_congruence(string eqs)
 {
 //list<congruent_equation> list;
 
 //list<int> cold_christmas::linear_congruence(string input)
 
 
 //system_of_congruence(list);
 
 return 0;
 }*/