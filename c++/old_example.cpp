#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int EffectiveCount(const vector<int>& v, int n, int i) {
    auto N = static_cast<int64_t>(v.size())*(i + 1)/(n + 1);
    if (N < log2(v.size())) {
        auto x = find_if(v.begin(), v.end(), [i] (int lhs) {
            return lhs > i;
        });
        cout << "Using find_if" << endl;
        return x - v.begin();
    }
    else {
        auto x = upper_bound(v.begin(), v.end(), i);
        cout << "Using upper_bound" << endl;
        return x - v.begin();
    }
    // место для вашего решения
}

int main() {
    static const int NUMBERS = 10000;
    static const int MAX = 4999;

    mt19937 r;
    uniform_int_distribution<int> uniform_dist(0, MAX);

    vector<int> nums;
    for (int i = 0; i < NUMBERS; ++i) {
        int random_number = uniform_dist(r);
        nums.push_back(random_number);
    }
    sort(nums.begin(), nums.end());

    int i;
    cin >> i;
    int result = EffectiveCount(nums, MAX, i);
    cout << "Total numbers before "s << i << ": "s << result << endl;
}