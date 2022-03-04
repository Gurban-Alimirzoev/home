#include <iostream>
#include <string>
#include <sstream>
using namespace std;

template <typename F>
string BruteForce(F check) {
    stringstream ss;
    string result, var;
    for (char a = 'A'; a < '['; a++) {
        for (char b = 'A'; b < '['; b++) {
            for (char c = 'A'; c < '['; c++) {
                for (char d = 'A'; d < '['; d++) {
                    for (char e = 'A'; e < '['; e++) {
                        var.push_back(a);
                        var.push_back(b);
                        var.push_back(c);
                        var.push_back(d);
                        var.push_back(e);
                        return result = (check(var) == 1 ? var : "0");
                        var.clear();
                        }
                }
            }
        }
    }
    return result;
    // верните строку str, для которой check(str) будет true
}

int main() {
    string pass = "ARTUR"s;
    auto check = [pass](const string& s) {
        return s == pass;
    };
    cout << BruteForce(check) << endl;
}