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

#include "log_duration.h"

using namespace std;

string GenerateWord(mt19937& generator, int max_length) {
    const int length = uniform_int_distribution(1, max_length)(generator);
    string word;
    word.reserve(length);
    for (int i = 0; i < length; ++i) {
        word.push_back(uniform_int_distribution('a', 'z')(generator));
    }
    return word;
}

template <template <typename> typename Container>
Container<string> GenerateDictionary(mt19937& generator, int word_count, int max_length) {
    vector<string> words;
    words.reserve(word_count);
    for (int i = 0; i < word_count; ++i) {
        words.push_back(GenerateWord(generator, max_length));
    }
    return Container(words.begin(), words.end());
}

struct Reverser {
    void operator()(string& value) const {
        reverse(value.begin(), value.end());
    }
};

template <typename Container, typename Function>
void Test(string_view mark, Container keys, Function function) {
    LOG_DURATION(mark);
    function(keys, Reverser{});
}

#define TEST(function) Test(#function, keys, function<remove_const_t<decltype(keys)>, Reverser>)

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    // ускорьте эту реализацию
    //for_each(execution::par, range.begin(), range.end(), function);
    const int threads = 2;    // количество задач/потоков
    auto count_elements = range.size(); // количество элементов в исходном контейнере
    // размер части массива для одной асинхронной задачи
    // если количество элементов не кратно количеству потоков то увеличиваем на 1 чтобы было кратное число
    size_t count_tasks = count_elements / threads + (count_elements % threads > 0 ? 1 : 0);
    auto begin_range = range.begin();   // начало части массива
    auto end_range = range.begin();     // конец части массива
    auto mid = next(begin_range, count_tasks);

    auto left_task = [function](auto iterF, auto iterL) {
        for (; iterF != iterL; iterF++) {
            function(*iterF);
        }
    };

    auto left_future = async([&function, left_task, begin_range, mid] { left_task(begin_range, mid); });
    auto right_future = async([&function, left_task, mid, end_range] { left_task(mid, end_range); });
}

int main() {
    // для итераторов с произвольным доступом тоже должно работать
    vector<string> strings = { "cat", "dog", "code" };

    ForEach(strings, [](string& s) {
        reverse(s.begin(), s.end());
        });

    for (string_view s : strings) {
        cout << s << " ";
    }
    cout << endl;
    // вывод: tac god edoc

    mt19937 generator;
    const auto keys = GenerateDictionary<list>(generator, 50'000, 5'000);

    TEST(ForEach);

    return 0;
}