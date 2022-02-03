#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end)
{
    for (auto it = range_begin; it != range_end; ++it)
    {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename It>
auto MakeVector(It range_begin, It range_end) {
    return vector(range_begin, range_end);
}

/*template <typename RandomIt>
RandomIt findMedian(RandomIt range_begin, RandomIt range_end)
{
    if (range_begin + 1 != range_end - 1 || range_begin != range_end-1)
    {
        findMedian(range_begin + 1, range_end - 1);
    }
    return range_begin;
}*/

template <typename RandomIt>
bool sortHand(RandomIt range_begin, RandomIt range_end) {
    if (*range_begin > range_begin[1]) {
        return true;
    }
    return false;
}

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
    RandomIt i = findMedian(range_begin, range_end);
    auto first = MakeVector(range_begin, i);
    auto second = MakeVector(i, range_end);
    if (sortHand(range_begin, range_begin + 1))
}

int main()
{
    vector<int> test_vector(10);

    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    iota(test_vector.begin(), test_vector.end(), 1);

    // random_shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // Перемешивает элементы в случайном порядке
    random_shuffle(test_vector.begin(), test_vector.end());

    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());

    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());

    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());

    return 0;
}
