#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <set>
#include <list>
#include <cassert>

using namespace std;

vector<string> MakeWithoutDot(string_view str)
{
    vector<string> result;
    size_t dot = str.find_first_of('.');

    if (dot == string_view::npos)
        result.push_back(string(str));

    while (dot != string_view::npos)
    {
        dot = str.find_first_of('.');
        result.push_back(string(str.substr(0, dot)));
        str = str.substr(dot + 1);
    }
    return result;
}

class Domain {
    // разработайте класс домена
public:
    Domain(string_view str)
        :domain_(MakeWithoutDot(str)), full_domain(str)
    {}
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами

    // разработайте operator==

    bool operator==(const Domain& domain)
    {
        return domain_ == domain.domain_;
    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& parent)
    {
        int parent_size = static_cast<int>(parent.domain_.size());
        int domain_size = static_cast<int>(domain_.size());

        if (parent_size > domain_size)
            return false;

        int diff_size = domain_size - parent_size;
        for (const string& parent_domain : parent.domain_)
        {
            if (domain_[diff_size++] != parent_domain)
                return false;
        }
        return true;
    }

    const vector<string>& GetDomain() const
    {
        return domain_;
    }

    const string& GetFullDomain() const
    {
        return full_domain;
    }

private:
    vector<string> domain_;
    string full_domain;
};

bool operator<(const Domain& lhs, const Domain& rhs)
{
    return lexicographical_compare(
        lhs.GetDomain().begin(),
        lhs.GetDomain().end(),
        rhs.GetDomain().begin(),
        rhs.GetDomain().end()
    );
}

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template <typename InputIt>
    DomainChecker(InputIt first, InputIt last)
        :blocked_domain(make_move_iterator(first), make_move_iterator(last))
    {}

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain)
    {
        auto iter = blocked_domain.find(domain);
        if (iter != blocked_domain.end() && (*iter).GetDomain().back() == domain.GetDomain().back())
            return true;
        else
            return false;
    }

private:
    set<Domain> blocked_domain;
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

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа

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
    string name_false = "com.exmaple";
    string name_sub_true = "ru.exmaple.com";
    string name_sub_false = "com.exmaple.ru";

    Domain domain_name_(name_);
    Domain domain_name_2(name_);
    Domain domain_name_false(name_false);
    assert(!(domain_name_ == domain_name_false));
    assert(domain_name_ == domain_name_2);


    Domain domain_name_sub_true(name_sub_true);
    Domain domain_name_sub_false(name_sub_false);
    assert(domain_name_sub_true.IsSubdomain(domain_name_));
    assert(!domain_name_sub_false.IsSubdomain(domain_name_));
}

void TestDomainCheker()
{
    vector<Domain> vector_ = { Domain("example.com"), Domain("student.com") };
    list<Domain> list_ = { Domain("ru"), Domain("student") };
    DomainChecker dc_vec(vector_.begin(), vector_.end());
    DomainChecker dc_list(list_.begin(), list_.end());

    assert(!dc_vec.IsForbidden(Domain("example_.ru")));
    assert(!dc_vec.IsForbidden(Domain("example")));

    assert(dc_vec.IsForbidden(Domain("example.com")));
    assert(!dc_vec.IsForbidden(Domain("e.com")));
}

void TestReadDomains()
{
    istringstream stream("example.com\n ru");
    auto result = ReadDomains(stream, 2);

    vector<Domain> vec = { Domain("example.com"), Domain("ru") };
    //assert(result == vec);
}

int main() {

    TestReadDomains();
    TestDomain();
    TestDomainCheker();

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}