#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <list>
#include <cassert>

using namespace std;

vector<string> MakeWithoutDot(string_view str)
{
    vector<string> result;
    size_t dot = str.find_first_of('.');
    while (dot == str.size())
    {
        result.push_back(string(str.substr(0, dot)));
        dot = str.find_first_of('.');
        str = str.substr(dot + 1);
    }
    result.push_back(string(str));
    return (result);
}

class Domain {
    // ������������ ����� ������
public:
    Domain(string_view str)
        :domain_(MakeWithoutDot(str))
    {}
    // ����������� ������ ��������� ��������������� �� string, � ���������� ������������ ����

    // ������������ operator==

    bool operator==(const Domain& domain)
    {
        return domain_ == domain.domain_;
    }

    // ������������ ����� IsSubdomain, ����������� ������ ����� � ������������ true, ���� this ��� ��������
    bool IsSubdomain(const Domain& parent)
    {
        int parent_size = static_cast<int>(parent.domain_.size());

        if (parent_size < static_cast<int>(domain_.size()))
            return false;

        for (const string& one_domain : domain_)
        {
            if (one_domain == parent.domain_[parent_size--])
                return true;
        }
        return false;
    }

    const vector<string>& GetDomain()
    {
        return domain_;
    }

private:
    vector<string> domain_;
};

class DomainChecker {
public:
    // ����������� ������ ��������� ������ ����������� ������� ����� ���� ����������
    template <typename InputIt>
    DomainChecker(InputIt first, InputIt last)
        :blocked_domain(first, last)
    {}

    // ������������ ����� IsForbidden, ������������ true, ���� ����� ��������
    bool IsForbidden(const Domain& domain)
    {
        blocked_domain.push_back(domain);
        return true;
    }

private:
    vector<Domain> blocked_domain;
};


const vector<Domain> ReadDomains(istream& input, size_t number)
{
    vector<Domain> result;
    string str;

    while (number != 0)
    {
        getline(input, str);
        result.push_back(Domain(str));
        number--;
    }
    return result;
}

// ������������ ������� ReadDomains, �������� �������� ���������� ������� �� ������������ �����

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

void TestDomain()
{
    string name_ = "exmaple.com";
    string name_false = "emaple.com";
    string name_sub = "ru.exmaple.com";

    Domain domain_name_(name_);
    Domain domain_name_false(name_false);
    Domain domain_name_sub(name_sub);
    Domain domain_name_true(name_);

    assert(!(domain_name_ == domain_name_false));
    assert(domain_name_ == domain_name_true);
    assert(domain_name_sub.IsSubdomain(domain_name_));
    assert(!domain_name_sub.IsSubdomain(domain_name_false));
}

void TestDomainCheker()
{
    vector<Domain> vector_ = { "good.example.com", "student.com" };
    list<Domain> list_ = { "ru", "student" };
    DomainChecker dc_vec(vector_.begin(), vector_.end());
    DomainChecker dc_list(list_.begin(), list_.end());

    assert(!dc_vec.IsForbidden(Domain("example_.ru")));
    assert(dc_vec.IsForbidden(Domain("example")));

    assert(dc_list.IsForbidden(Domain("example1.ru")));
    assert(!dc_list.IsForbidden(Domain("example.com")));
}

void TestReadDomains()
{
    istringstream stream("example.com\n ru");
    auto result = ReadDomains(stream, 2);

    vector<Domain> vec = { Domain("example.com"), Domain("ru") };
    //assert(result == vec);
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}