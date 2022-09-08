#include <iostream>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <numeric>
using namespace std;

class HotelManager {
public:
    void Book(int64_t time, string hotel_name, int client_id, int room_count)
    {
        db_[hotel_name].push_back({ time , client_id });
        time_and_sum[hotel_name].push_back({ time , room_count });
        current_time = time;
    }

    int ComputeClientCount(string hotel_name)
    {
        deque<pair<int64_t, int>> dict = db_[hotel_name];
        int64_t one_day = current_time - 86400;
        remove_if(dict.begin(), dict.end(),
            [one_day](pair<int64_t, int> pair_)
            {
                return (pair_.first - one_day) <= 0;
            });
        return static_cast<int>(dict.size());
    }

    int ComputeRoomCount(string hotel_name)
    {
        deque<pair<int64_t, int>> dict = time_and_sum[hotel_name];
        int64_t one_day = current_time - 86400;
        auto noSpaceEnd = remove_if(dict.begin(), dict.end(),
            [one_day](pair<int64_t, int> pair_)
            {
                return (pair_.first - one_day) <= 0;
            });
        dict.erase(noSpaceEnd, dict.end());
        int result = 0;
        for (auto [time, rooms] : dict)
        {
            result += rooms;
        }
        return result;
    }

private:
    map <string, deque<pair<int64_t, int>>> db_;
    map <string, deque<pair<int64_t, int>>> time_and_sum;
    int64_t current_time = 0;
};

int main() {
    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        }
        else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            }
            else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }

    return 0;
}