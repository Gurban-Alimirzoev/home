#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <list>
#include <cassert>

using namespace std;

vector<string>&& MakeWithoutDot(string_view str)
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
    return move(result);
}

class Domain {
    // разработайте класс домена
public:
    Domain(string_view str)
        :domain(MakeWithoutDot(str))
    {}
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами

    // разработайте operator==

    bool operator==(const Domain& domain)
    {

    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(Domain parent)
    {}

private:
    vector<string> domain;
};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template <typename InputIt>
    DomainChecker(InputIt first, InputIt last)
    {}

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain)
    {
    }
};


const vector<Domain> ReadDomains(istream& input, size_t number)
{

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
    istringstream stream("example.com, ru");
    auto result = ReadDomains(stream, 2);

    vector<Domain> vec = { Domain("example.com"), Domain("ru") };
    assert(result == vec);
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}