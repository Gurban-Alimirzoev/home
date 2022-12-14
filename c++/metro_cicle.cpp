#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

using std::cin;
using std::cout;

int MetroCicle(int station_number = 0,
	int start = 0,
	int finish = 0)
{
    if (finish - start == 1)
        return 0;
    if (station_number - finish == 0 && start == 1)
        return 0;
	int result = finish - start < station_number - finish ? finish - start : station_number - finish;	
	return result;	
}

int main()
{
	int station_number = 0;
	int start = 0;
	int finish = 0;
	cin >> station_number >> start >> finish;
	cout << MetroCicle(station_number, start, finish);
}