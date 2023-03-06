#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>

using std::cin;
using std::cout;

struct Coordinates
{
	int32_t x = 0;
	int32_t y = 0;
};

class Graph
{
	public:
	void Get();

	private:
	std::set<Coordinates> nodes;
};

int32_t FindRoad(std::vector<Coordinates> cites, int32_t fule, int32_t start, int32_t finish)
{
	
}

int main()
{
	int32_t number_of_city;
	cin >> number_of_city;
	std::vector<Coordinates> cites(number_of_city);
	Coordinates coor;
	for(int32_t i = 0; i < number_of_city; i++)
	{		
		cin >> coor.x >> coor.y;
		cites [i] = coor;
	}
	int32_t fule = 0;
	int32_t start = 0;
	int32_t finish = 0;
	cout << FindRoad(cites, fule, start, finish);
}