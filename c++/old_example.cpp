#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using std::cin;

bool CheckNumber(int& number)
{
	return number > 0;
}

void CinNumber(int& number, int& int_)
{
	if (CheckNumber(number))
	{
		cin >> int_;
		number--;
	}
}

void Example(int& number, std::vector<int>& numbers)
{	
	int prev = 0;
	int cur = 0;
	CinNumber(number, prev);
	while (CheckNumber(number))
	{
		CinNumber(number, cur);
		if (prev ^ cur)
		{
			numbers.push_back(prev);
		}
		prev = cur;
	}

	if (!numbers.empty() && numbers.back() != prev)
	{
		numbers.push_back(prev);
	}
}

int main()
{
	int number = 0;
	cin >> number;
	std::vector<int> result;
	Example(number, result);
	for (auto c : result)
	{
		std::cout << c << std::endl;
	}
}