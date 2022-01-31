#include <string>
#include <vector>
#include <iostream>
#include <numeric>
using namespace std;

int square_digits(int num)
{
    std::string result;
    int var;
    for (char i : std::to_string(num))
    {
        var = ((int)i - (int)'0') * ((int)i - (int)'0');
        result += (to_string(var));
    }

    return stoi(result);
}

int main()
{
    int x = square_digits(1229);
    cout << x;
}