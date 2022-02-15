#include <vector>
#include <numeric>
#include <iterator>

using namespace std;

class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        double average = accumulate(nums.begin(), next(nums.begin(), k), 0.0);
        double result = average, newAve;
        for (auto i = nums.begin(); i < (nums.end() - k); i++) {
            newAve = average - *i + *(i + k);
            if (result < newAve) {
                result = newAve;
            }
            average = newAve; 
        }
        return result/k;
    }
};

int main() {
    std::vector<int> vec = {1,12,-5,-6,50,3};
    Solution Sol;
    Sol.findMaxAverage(vec, 4);
}