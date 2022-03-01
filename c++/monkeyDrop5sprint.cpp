#include <iostream>

using namespace std;

template <typename F>
int FindFloor(int n, F drop) {
    int a = 0, b = n, m;
    int varA, varB;
    bool result;
    do {
        m = (a + b) / 2;
        varA = a;
        varB = b;
        result = drop(m);
        b = (result == 1 ? m : varB);
        a = (result == 0 ? m + 1 : varA); 
    }
    while (a != b);
    return a;
}

int main() {
    int n,t;
    cout << "Enter n and target floor number: "s << endl;
    cin >> n >> t;

    int count = 0;
    int found = FindFloor(n, [t, &count](int f) {
        ++count;
        return f >= t;
    });

    cout << "Found floor "s << found << " after "s << count << " drops"s;

    return 0;
}