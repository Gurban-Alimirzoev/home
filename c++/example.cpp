#include <numeric>
#include <cstdlib>
using namespace std;

class Rational {
public:
    Rational() = default;
    
    Rational(int iter) {
        numerator_ = iter;
        denominator_ = 1;
    }
    
    Rational (int iter, int denom) {
        int del = gcd (iter, denom);
        if (del != 0) {
            numerator_ = (iter) / del;
            denominator_ = (denom) / del;
        }
        else {
            numerator_ = iter;
            denominator_ = denom;
        }
    }
    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    int numerator_ = 0;
    int denominator_ = 1;
};

int main() {
    const Rational scale(4, 6);
}