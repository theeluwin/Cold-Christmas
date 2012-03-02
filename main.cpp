#include "cold_christmas.cpp"

using namespace CC;

int main(int argc, char *argv[])
{
	string input;
	while(1)
	{
		std::getline(std::cin, input);
		std::cout << CC::compute(input);
	}
	return 0;
}