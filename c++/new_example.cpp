#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end)
{
    for (auto it = range_begin; it != range_end; ++it)
    {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Container, typename It>
void EraseAndPrint(Container cont, It it)
{
    auto oldBegin = cont.begin();
    auto oldEnd = cont.end();
    if (it != oldBegin)
    {
        cont.erase(it);
        PrintRange(cont.begin(), it++);
        PrintRange(it++, cont.end());
    }
    else {
        cont.erase(it);
        cout << endl;
        PrintRange(cont.begin(), cont.end()); 
    }
}

int main()
{
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    EraseAndPrint(langs, langs.begin());
}