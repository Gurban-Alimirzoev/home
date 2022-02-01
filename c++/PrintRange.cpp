#include <set>
#include <iostream>
using namespace std;

#include <iostream>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}
int main()
{
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    PrintRange(test.begin(), test.end());
}