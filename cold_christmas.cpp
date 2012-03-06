// Standards
#include <iostream>
#include <sstream>
#include <cmath>

// STL
#include <list>
#include <stack>
#include <string>
#include <algorithm>

// Namespace
using std::list;
using std::string;

// Constant
const int P_MAX = 10000;

namespace CC
{
	// Variables & Initializer
	static bool is_prime[P_MAX];
	static list<int> create_primes()
	{
		list<int> ret;
		for(int i = 2; i < P_MAX; i++) is_prime[i] = true;
		is_prime[0] = is_prime[1] = false;
		for(int i = 2; i < P_MAX; i++)
			if(is_prime[i])
			{
				ret.push_back(i);
				for(int j = i + i; j < P_MAX; j += i)
					is_prime[j] = false;
			}
		return ret;
	}
	static list<int> primes = create_primes();
	
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
	};

	// Normal Modules
	template <typename T>
	static string print(list<T> _list)
	{
		if(_list.size() == 0) return "";
		std::stringstream ret;
		for(typename list<T>::iterator it = _list.begin(); it != _list.end();)
		{
			ret << *it;
			it++;
			if(it != _list.end()) ret << ", ";
		}
		ret << std::endl;
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
		for(list<int>::iterator it = primes.begin(); n > 1; it++)
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

	// Chinease Remainder Theorem
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

					list<int> sol1 = linear_congruence(*it);
					list<int> sol2 = linear_congruence(*it2);
					list<int> temp;

					for(int i = 1; i < M / it->n; i++) // i : 1 to 3-1=2
					{
						for(list<int>::iterator j = sol1.begin(); j != sol1.end(); j++)
							temp.push_back(*j + it->n * i);
					}
					sol1.splice(sol1.end(), temp);
					temp.clear();

					for(int i = 1; i < M / it2->n; i++)
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
								*it = congruent_equation(1, *i, M);
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
//				it2++;
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
			list<int> t = linear_congruence(congruent_equation(it->a, it->b, it->n));
			int s = N / it->n;
			int u = inverse(N / it->n, it-> n);
			ret += t.front() * s * u;
		}

		return ret % N;
	}

	// String Parsers
	static list<string> tokenize(string input, char token)
	{
		list<string> ret;
		int n = (int)input.length();
		string temp = "";
		for(int i = 0; i < n; i++)
		{
			if(input[i] != token) temp += input[i];
			else
			{
				ret.push_back(temp);
				temp = "";
			}
		}
		ret.push_back(temp);
		return ret;
	}
	static list<string> pairs_from_string(string str)
	{
		list<string> ret;
		int n = (int)(str.length());
		for(int i = 0; i < n - 1; i++)
			ret.push_back(str.substr(i, 2));
		return ret;
	}
	static int similarity(string str1, string str2)
	{
		list<string> pairs1 = pairs_from_string(str1);
		list<string> pairs2 = pairs_from_string(str2);
		int total_length = (int)(str1.length() + str2.length());
		int hit_count = 0;
		for(list<string>::iterator it1 = pairs1.begin(); it1 != pairs1.end(); it1++)
			for(list<string>::iterator it2 = pairs2.begin(); it2 != pairs2.end(); it2++)
				if(*it1 == *it2)
					hit_count++;
		return (2 * hit_count * 100 / total_length);
	}
	static int read_first_number(string input)
	{
		string temp = "";
		int i = 0, n = (int)input.length();

		for(i = 0; i < n; i++) if(is_num(input[i])) break;
		for(; i < n; i++)
			if(is_num(input[i])) temp += input[i];
			else break;

		if(temp.length() != 0)
			return string_to_int(temp);
		else
			return 0;
	}
	static congruent_equation string_to_equation(string input)
	{
		int a, b, n, i;

		int len = (int)input.length();
		for(i = 0; i < len; i++)
			if(input[i] == '=') break;

		string temp = input.substr(0, i);
		a = read_first_number(temp);
		if(a == 0) a = 1;

		while(!is_num(input[i])) i++;
		b = read_first_number(input.substr(i, input.length() - i));
		while(!is_num(input[i])) i++;
		while(is_num(input[i])) i++;
		n = read_first_number(input.substr(i, input.length() - i));

		return congruent_equation(a, b, n);
	}
	static string compute(string input)
	{
		int n = (int)input.length();
		if(n == 0) return "";
		int chinease = 0;
		string sub;
		for(int i = 0; i < n; i++)
		{
			// Check if input is asking for primitive root.
			sub = input.substr(i, 14);
			if(similarity(sub, "primitive root of") > 60)
				return print(primitive_root(read_first_number(input.substr(i, n-i))));

			// If not, check if there is term "mod".
			sub = input.substr(i, 3);
			if(sub == "mod")
				chinease++;
		}
		if(chinease > 1)
		{
			list<string> eqs_string = tokenize(input, ',');
			list<congruent_equation> eqs;
			for(list<string>::iterator it = eqs_string.begin(); it != eqs_string.end(); it++)
				eqs.push_back(string_to_equation(*it));
			int ret_int = system_of_congruence(eqs);
			list<int> ret_list;
			ret_list.push_back(ret_int);
			return print(ret_list);
		}
		else
			return print(linear_congruence(string_to_equation(input)));
		return "No matching expression.";
	}
}