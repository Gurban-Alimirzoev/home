#include <set>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}


template <typename Container, typename ContainerType>
void FindAndPrint(Container cont, ContainerType it)
{
    auto iter = find(cont.begin(), cont.end(), it);
    if (iter != cont.end()) {
        PrintRange(cont.begin(), iter);
        PrintRange(iter, cont.end());
    }
    else {
        PrintRange(cont.begin(), cont.end());
        }
}

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    FindAndPrint(test, 3);
    FindAndPrint(test, 0);
} 
