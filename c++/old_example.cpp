#include <iostream>
#include <list>
#include <set>
#include <string_view>
#include <vector>
#include <span>

using namespace std;

// »змените сигнатуру этой функции, чтобы она могла
// сливать не только векторы, но и любые другие контейнеры
template <typename InputIt1, typename InputIt2>
void Merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, ostream& out) {
    size_t i1 = 0, i2 = 0;
    while (i1 < src1.size() && i2 < src2.size()) {
        if (src1[i1] < src2[i2]) {
            out << src1[i1++] << endl;
        }
        else {
            out << src2[i2++] << endl;
        }
    }
    while (i1 < src1.size()) {
        out << src1[i1++] << endl;
    }
    while (i2 < src2.size()) {
        out << src2[i2++] << endl;
    }
}

template <typename T, typename S>
void MergeSomething(T src1, S src2, ostream& out) {
    Merge(src1, src2, out);
}

// –еализуйте эту функцию:
template <typename T>
void MergeHalves(T src, ostream& out) {
    size_t mid = (src.size() + 1) / 2;
    // —делать Merge участка вектора от 0 до mid и от mid до конца.
    // Ёлемент с индексом mid включаетс€ во второй диапазон.
}

int main() {
    vector<int> v1{ 60, 70, 80, 90 };
    vector<int> v2{ 65, 75, 85, 95 };
    vector<int> combined{ 60, 70, 80, 90, 65, 75, 85, 95 };
    list<double> my_list{ 0.1, 72.5, 82.11, 1e+30 };
    string_view my_string = "ACNZ"sv;
    set<unsigned> my_set{ 20u, 77u, 81u };

    // пока функци€ MergeSomething реализована пока только дл€ векторов
    cout << "Merging vectors:"sv << endl;
    MergeSomething(v1, v2, cout);

    cout << "Merging vector and list:"sv << endl;
    MergeSomething(v1, my_list, cout);

    cout << "Merging string and list:"sv << endl;
    MergeSomething(my_string, my_list, cout);

    cout << "Merging set and vector:"sv << endl;
    MergeSomething(my_set, v2, cout);

    cout << "Merging vector halves:"sv << endl;
    MergeHalves(combined, cout);
}