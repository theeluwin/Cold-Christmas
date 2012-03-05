'Primes Constant'
import os

f = open("/".join(os.path.realpath(__file__).split('/')[:-1] + ['primes']), 'r')
primes = map(lambda x: int(x), f.read().replace('\n', '').split(' '))

'Struct Prototypes'
class prime_factor:

	def __init__(self, r, e):
		self.radix = r
		self.exp = e

	def __repr__(self):
		return "%d ** %d" % (self.radix, self.exp)

class linear_equation:

	def __str__(self):
		return self.a + '*' + self.x + '+' + self.y + '=' + self.c

	def __init__(self, a = 0, x = 0, b = 0, y = 0, c = 0):
		self.a = a
		self.x = x
		self.b = b
		self.y = y
		self.c = c

	def __getitem__(self, key):
		return [self.a, self.x, self.b, self.y, self.c][key]

	def __add__(self, e):
		return linear_equation(e.c, self.y, self.a, self.x - e.x * self.y, self.c)

class congruent_equation:

	def __init__(self, a, b, n):
		self.a = a
		self.b = b
		self.n = n
		
	def __getitem__(self, key):
		return [self.a, self.b, self.n][key]

	def __repr__(self):
		return "%d (mod %d)" % (self.b, self.n)

'Math Modules'
from fractions import gcd, lcm

def relative_prime(n):
	if n == 2:
		return [1]

	ret = []

	for i in range(1, int(n / 2) + 1):
		if gcd(i, n) == 1:
			ret.append(i)
			ret.append(n - i)

	ret.sort()
	return ret

def prime_factorization(n):
	ret = []

	for it in primes:
		if not n > 1:
			break
		if n % it == 0:
			ret.append(prime_factor(it, 0))
			while n % it == 0:
				n /= it
				ret[-1].exp += 1

	return ret

def divisor(n):
	ret = [1]

	for div in prime_factorization(n):
		temp = ret[:]
		for it in temp:
			for i in range(div.exp, 0, -1):
				ret.append((div.radix ** i) * it)

	ret.sort()
	return ret

def phi(n):
	return len(relative_prime(n))

def inverse(a, n):
	if a == 1:
		return 1

	'Phase 0 : Determine if the congruence has a solution.'
	if gcd(a, n) > 1:
		raise Exception('has no inverse')

	'Get the solution by using Euclidian Algorithm.'

	'Phase 1 : Push linear equations while the remainder is chosen.'
	stack = []
	stack.append(linear_equation(a, int(n / a), int(n % a), 1, n))

	while stack[-1].b != 0:
		top = stack[-1]
		stack.append(linear_equation(top.b, int(top.a / top.b), top.a % top.b, 1, top.a))

	'Phase 2 : Pop'
	stack.pop()

	eq = linear_equation(stack[-1].c, 1, stack[-1].a, -stack[-1].x, 1)
	stack.pop()

	while len(stack) > 0:
		eq += stack[-1]
		stack.pop()

	if eq.a == a:
		return (eq.x % n + n) % n

	'else'
	return (eq.y % n + n) % n

def mod_pow(r, p, n):
	return r ** p % n

'Primitive Root'
def primitive_root(n):
	pf = prime_factorization(n)
	ret = []

	'Phase 0 : Determine if n has a primitive root'
	'   only 2, 4, p ** k or 2 * p ** k have primitive roots'

	if n != 2 and n != 4 and not (len(pf) == 1 and pf[0].radix != 2) and not (len(pf) == 2 and pf[0].radix == 2 and pf[0].exp == 1):
		raise Exception('The integer has no primitive root')

	'Phase 1 : Find the smallest primitve root'
	divisors_of_phi = divisor(phi(n))[:-1]
	relative_primes = relative_prime(n)

	for it in relative_primes:
		flag = False
		for it2 in divisors_of_phi:
			if mod_pow(it, it2, n) == 1:
				flag = True

		r = it
		if not flag:
			break

	'Phase 2 : Return the list of the r ** p in relative_prime(phi(n))'
	for it in relative_prime(phi(n)):
		ret.append(mod_pow(r, it, n))

	ret.sort()
	return ret



'Chinease Remainder Theorm'

def linear_congruence(eq):
	ret = []
	a = eq.a
	b = eq.b
	n = eq.n

	'phase 0 : Determine if the congruence has a solution.'
	d = gcd(a, n)
	if b % d:
		raise Exception("the congruence has no solution")
	else:
		'phase 1 : Determine if (a, n) = 1. It not, divide a and n by (a, n).'
		if d > 1:
			a /= d
			b /= d
			n /= d

		'phase 2 : Find the special solution of ax = 1 (mod n) by solving ax - ny = 1.'
		a = a % n
		sol = (inverse(a, n) * b ) % n

		'phase 3 : return the all solutions'
		for i in range(d - 1, -1, -1):
			ret.append(sol + i * n)

	ret.sort()
	return ret

def system_of_congruence(eqs):
	N = 1

	'phase 1 : if two congruences has gcd > 1, merge them'
	#mergedEquations = []
	#isMerged = []
	#for i in range(len(eqs)):
	#	isMerged.append(False)
	e = 0
	while e < len(eqs) - 1:
		f = e + 1
		while f < len(eqs):
			eq1 = eqs[e]
			eq2 = eqs[f]
			if gcd(eq1.n, eq2.n) != 1:
				M = lcm(eq1.n, eq2.n)
				sol1 = linear_congruence(eq1)
				sol2 = linear_congruence(eq2)
				temp = []
				for i in range(1, M / eq1.n):
					for ans in sol1:
						temp.append(ans + eq1.n * i)
				sol1 += temp
				temp = []
				for i in range(1, N / eq2.n):
					for ans in sol2:
						temp.append(ans + eq2.n * i)
				sol2 += temp
				flag = True
				for i in sol1:
					if not flag:
						break
					for j in sol2:
						if not flag:
							break
						if i == j:
							eqs[e] = congruent_equation(1, i, N)
							del eqs[f]
							f -= 1
							flag = False
				if flag:
					raise Exception("the congruences have no solution")
			f += 1
		e += 1

	'phase 2 : set the N'
	for eq in eqs:
		N *= eq.n
	
	'phase 3 : now every n is relative prime each other. return the solution'
	ret = 0
	for eq in eqs:
		temp = linear_congruence(congruent_equation(N/eq.n, eq.b, eq.n))[0]
		ret += temp * N / eq.n

	return congruent_equation(1, ret % N, N)

def pairs_from_string(str):
	ret = []
	for i in range(0, len(str) - 1):
		ret.append(str[i:i+2])
	return ret

def similarity(str1, str2):
	pairs1 = pairs_from_string(str1)
	pairs2 = pairs_from_string(str2)
	total_length = len(str1) + len(str2)
	hit_count = 0
	for piece1 in pairs1:
		for piece2 in pairs2:
			if piece1 == piece2:
				hit_count += 1

	return 2.0 * hit_count * 100 / total_length

def read_first_number(input):
	ret = ""

	for token in input:
		if token.isdigit():
			ret += token
		elif ret is not "":
			return int(ret)

	if ret == "":
		return 0
	return int(ret)

def string_to_equation(input):
	left_hand, right_hand = input.split('=')

	a = read_first_number(left_hand)
	if a == 0:
		a = 1
	b = read_first_number(right_hand)
	n = read_first_number(right_hand[right_hand.index("%d" % b) + len("%d" % b):])

	return congruent_equation(a, b, n)


def compute(input):
	if input == "":
		raise Exception("No input")

	for index in range(0, len(input) - 14):
		if similarity(input[index:index + 14], 'primitive root of') > 60:
			return primitive_root(read_first_number(input[index:]))

	if input.count('mod') > 1:
		eqs_string = input.split(',')
		eqs = map(lambda equation: string_to_equation(equation), eqs_string)

		return system_of_congruence(eqs)
	else:
		return linear_congruence(string_to_equation(input))

	raise Exception("No matching expression")

def test_expression(name, expr, correct_value):
	print "%s test %r, value is %r" % (name, expr == correct_value, expr)

def test():
	test_expression('linear_congruence', linear_congruence(congruent_equation(35, 1, 3))[0], 2)
	test_expression('known', compute('x = 1 mod 3, x = 2 mod 5, x = 3 mod 7')[0][1], 52)
