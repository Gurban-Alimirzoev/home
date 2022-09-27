#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
    // ������������ ����� ������

    // ����������� ������ ��������� ��������������� �� string, � ���������� ������������ ����

    // ������������ operator==

    // ������������ ����� IsSubdomain, ����������� ������ ����� � ������������ true, ���� this ��� ��������
};

class DomainChecker {
public:
    // ����������� ������ ��������� ������ ����������� ������� ����� ���� ����������

    // ������������ ����� IsForbidden, ������������ true, ���� ����� ��������
};

// ������������ ������� ReadDomains, �������� �������� ���������� ������� �� ������������ �����

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}