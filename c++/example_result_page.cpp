#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;
/*
template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, string prefix)
{
    string s = , spre = prefix;
    //s = static_cast<char>(prefix + 1);
    //spre = static_cast<char>(prefix);

    RandomIt iter = lower_bound(range_begin, range_end, spre);
    RandomIt iterNext = lower_bound(iter, range_end, s);
    if (iter == range_end)
    {
        return pair{iterNext, iterNext};
    }
    else
    {
        return pair{iter, iterNext};
    }
}*/

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, string prefix) {

    auto left = lower_bound(range_begin, range_end, prefix);

    char next_prefix = static_cast<char>(*(prev(prefix.end(), 1)) + 1);
    
    *(prev(prefix.end(), 1)) = next_prefix;
    auto right = lower_bound(range_begin, range_end, prefix);

    return {left, right};
}

int main() {
    const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};

    const auto mo_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
    for (auto it = mo_result.first; it != mo_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto mt_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
    cout << (mt_result.first - begin(sorted_strings)) << " " << (mt_result.second - begin(sorted_strings)) << endl;

    const auto na_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
    cout << (na_result.first - begin(sorted_strings)) << " " << (na_result.second - begin(sorted_strings)) << endl;

    return 0;
}