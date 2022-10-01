#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

class RouteManager {
public:
	RouteManager& AddRoute(int start, int finish) {
		reachable_lists_[start].insert(finish);
		reachable_lists_[finish].insert(start);
		return *this;
	}

	int FindNearestFinish(int start, int finish) const {

		int result = abs(start - finish);
		if (!reachable_lists_.count(start))
			return result;

		const set<int>& reachable_stations = reachable_lists_.at(start);
		if (reachable_stations.count(finish))
			return 0;

		auto it = reachable_stations.lower_bound(finish);

		if (it == reachable_stations.end())
			return (min(result, abs(*prev(it) - finish)));
		return min({ result, abs(finish - *it), abs(finish - *prev(it)) });
	}

private:
	map<int, set<int>> reachable_lists_;
};

void VariantsTests() {

	// YandexLessenTest
	{
		RouteManager routes;
		routes.AddRoute(-2, 5).AddRoute(10, 4).AddRoute(5, 8);

		assert(routes.FindNearestFinish(4, 10) == 0);
		assert(routes.FindNearestFinish(4, -2) == 6);
		assert(routes.FindNearestFinish(5, 0) == 2);
		assert(routes.FindNearestFinish(5, 100) == 92);
	}

	{
		RouteManager routes;
		routes.AddRoute(-2, 5).AddRoute(5, 15).AddRoute(-10, 5);
		routes.AddRoute(10, 4).AddRoute(4, -7).AddRoute(-1, 10);
		routes.AddRoute(5, 8).AddRoute(17, 5).AddRoute(-18, 5);
		routes.AddRoute(5, 1).AddRoute(33, 5).AddRoute(-3, 5);

		assert(routes.FindNearestFinish(4, 10) == 0);     // есть такой маршрут
		assert(routes.FindNearestFinish(4, -2) == 5);     // ближайшая станция -7
		assert(routes.FindNearestFinish(5, 0) == 1);      // ближайшая станция 1
		assert(routes.FindNearestFinish(5, 100) == 67);   // ближайшая станция 33
		assert(routes.FindNearestFinish(5, -8) == 2);     // ближайшая станция -10
		assert(routes.FindNearestFinish(5, 17) == 0);     // есть такой маршрут
		assert(routes.FindNearestFinish(5, 37) == 4);     // ближайшая станция 33
		assert(routes.FindNearestFinish(5, -13) == 3);    // ближайшая станция -10
		assert(routes.FindNearestFinish(5, -15) == 3);    // ближайшая станция -18
		assert(routes.FindNearestFinish(5, -14) == 4);    // граничный случай между -10 и -18
	}

	{
		RouteManager routes;
		routes.AddRoute(-658453724, -346143785);
		assert(routes.FindNearestFinish(-658453724, -61896097) == 284247688);
	}

	std::cerr << "All test passed!" << std::endl;
}

int main() {
	//VariantsTests();
	RouteManager routes;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;
		int start, finish;
		cin >> start >> finish;
		if (query_type == "ADD"s) {
			routes.AddRoute(start, finish);
		}
		else if (query_type == "GO"s) {
			cout << routes.FindNearestFinish(start, finish) << "\n"s;
		}
	}
}