#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

std::vector<int> Example(std::vector<int> numbers)
{
}

int main()
{
	int number = 0;
	std::cin >> number;
	int prev = 0;
	int cur = 0;
	std::cin >> prev >> cur;
	number -= 2;
	std::vector<int> result;
	while (number >= 0)
	{
		if (prev ^ cur)
		{
			result.push_back(prev);
			prev = cur;
		}
		else
		{
			if (--number < 0)
			{
				break;
			}
			std::cin >> prev;
		}
		if (--number < 0)
		{
			break;
		}
		std::cin >> cur;
	}
	if (result.back() != prev)
	{
		result.push_back(prev);
	}
	for (auto c : result)
	{
		std::cout << c << std::endl;
	}
}