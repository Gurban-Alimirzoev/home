#include <iostream>
#include <list>
#include <set>
#include <string_view>
#include <vector>
#include <iterator>

using namespace std;

// �������� ��������� ���� �������, ����� ��� �����
// ������� �� ������ �������, �� � ����� ������ ����������
template <typename InputIt1, typename InputIt2>
void Merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, ostream& out) {
    InputIt1 i1 = first1;
    InputIt2 i2 = first2;
    while (i1 != last1 && i2 != last2) {
        if (less<>()(*i1, *i2)) {
            out << *i1 << endl;
            i1 = next(i1);
        }
        else {
            out << *i2 << endl;
            i2 = next(i2);
        }
    }
    while (i1 != last1) {
        out << *i1 << endl;
        i1 = next(i1);
    }
    while (i2 != last2) {
        out << *i2 << endl;
        i2 = next(i2);
    }
}

template <typename T, typename S>
void MergeSomething(const T& src1, const S& src2, ostream& out) {
    Merge(src1.begin(), src1.end(), src2.begin(), src2.end(), out);
}

// ���������� ��� �������:
template <typename T>
void MergeHalves(const vector<T>& src, ostream& out) {
    size_t mid = (src.size() + 1) / 2;
    auto mid_iter = next(src.begin(), mid);
    Merge(src.begin(), mid_iter, mid_iter, src.end(), out);
}

int main() {
    vector<int> v1{ 60, 70, 80, 90 };
    vector<int> v2{ 65, 75, 85, 95 };
    vector<int> combined{ 60, 70, 80, 90, 65, 75, 85, 95 };
    list<double> my_list{ 0.1, 72.5, 82.11, 1e+30 };
    string_view my_string = "ACNZ"sv;
    set<unsigned> my_set{ 20u, 77u, 81u };

    // ���� ������� MergeSomething ����������� ���� ������ ��� ��������
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