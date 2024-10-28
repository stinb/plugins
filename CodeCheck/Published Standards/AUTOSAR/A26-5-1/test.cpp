// $Id: A26-5-1.cpp 311495 2018-03-13 13:02:54Z michal.szczepankiewicz $
// g++ -Wconversion-null -std=c++14 main.cpp -o main.exe
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <random>
#include <algorithm>

int main()
{
	std::srand(std::time(nullptr)); // UndCC_Violation(1)
	int r1 = std::rand() % 100; // UndCC_Violation
	std::cout << "Random value using std::rand(): " << r1 << std::endl;

	std::random_device rd;
	std::default_random_engine eng{rd()};
	std::uniform_int_distribution<int> ud{0, 100};
	int r2 = ud(eng); // UndCC_Valid
	std::cout << "Random value using std::random_device: " << r2 << std::endl;

	int r3 = 0;
	if (std::rand() % 10 > r3) // UndCC_Violation
	{
		std::cout << r3 << std::endl;
	}

	std::vector<int> myvector;
	for (int i = 1; i < 10; ++i)
		myvector.push_back(i);
	std::random_shuffle(myvector.begin(), myvector.end()); // UndCC_Violation(0)

	return 0;
}
