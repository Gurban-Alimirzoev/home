#include <algorithm>
#include <execution>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <future>
#include <iterator>
#include <type_traits>

#include "log_duration.h"

using namespace std;

string GenerateWord(mt19937 &generator, int max_length)
{
    const int length = uniform_int_distribution(1, max_length)(generator);
    string word;
    word.reserve(length);
    for (int i = 0; i < length; ++i)
    {
        word.push_back(uniform_int_distribution('a', 'z')(generator));
    }
    return word;
}

template <template <typename> typename Container>
Container<string> GenerateDictionary(mt19937 &generator, int word_count, int max_length)
{
    vector<string> words;
    words.reserve(word_count);
    for (int i = 0; i < word_count; ++i)
    {
        words.push_back(GenerateWord(generator, max_length));
    }
    return Container(words.begin(), words.end());
}

struct Reverser
{
    void operator()(string &value) const
    {
        reverse(value.begin(), value.end());
    }
};

template <typename Container, typename Function>
void Test(string_view mark, Container keys, Function function)
{
    LOG_DURATION(mark);
    function(keys, Reverser{});
}

#define TEST(function) Test(#function, keys, function<remove_const_t<decltype(keys)>, Reverser>)

template <typename Iterator>
struct is_random_access_iterator
    : std::is_same<
          typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>
{
};

template <typename Container>
struct has_random_access_iterator
    : is_random_access_iterator<typename Container::iterator>
{
};

template <typename ForwardRange, typename Function>
void ForEach(execution::parallel_policy policy, ForwardRange &range, Function function)
{
    // ускорьте эту реализацию
    // for_each(execution::par, range.begin(), range.end(), function);

    if (has_random_access_iterator<ForwardRange>::value)
    {
        for_each(policy, range.begin(), range.end(), function);
    }
    else
    {

        const int threads = 2;              // количество задач/потоков
        auto count_elements = range.size(); // количество элементов в исходном контейнере
        // размер части массива для одной асинхронной задачи
        // если количество элементов не кратно количеству потоков то увеличиваем на 1 чтобы было кратное число
        size_t count_tasks = count_elements / threads + (count_elements % threads > 0 ? 1 : 0);
        auto begin_range = range.begin(); // начало части массива
        auto end_range = range.end();     // конец части массива
        // auto mid = next(begin_range, count_tasks);

        auto left_task = [function](auto iterF, auto iterL)
        {
            for (; iterF != iterL; iterF++)
            {
                function(*iterF);
            }
        };

        auto left_future = async([&function, left_task, begin_range, count_tasks]
                                 { left_task(begin_range, next(begin_range, count_tasks)); });
        auto right_future = async([&function, left_task, begin_range, count_tasks, end_range]
                                  { left_task(next(begin_range, count_tasks), end_range); });
    }
}

template <typename ForwardRange, typename Function>
void ForEach(execution::sequenced_policy policy, ForwardRange &range, Function function)
{
    for_each(policy, range.begin(), range.end(), function);
}

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange &range, Function function)
{
    ForEach(execution::seq, range, function);
}

using namespace std;

template <typename Strings>
void PrintStrings(const Strings &strings)
{
    for (string_view s : strings)
    {
        cout << s << " ";
    }
    cout << endl;
}

int main()
{
    auto reverser = [](string &s)
    { reverse(s.begin(), s.end()); };

    list<string> strings_list = {"cat", "dog", "code"};

    ForEach(strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    ForEach(execution::seq, strings_list, reverser);
    PrintStrings(strings_list);
    // cat dog code

    // единственный из вызовов, где должна работать ваша версия
    // из предыдущего задания
    ForEach(execution::par, strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    vector<string> strings_vector = {"cat", "dog", "code"};

    ForEach(strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    ForEach(execution::seq, strings_vector, reverser);
    PrintStrings(strings_vector);
    // cat dog code

    ForEach(execution::par, strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    return 0;
}