#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
 
using namespace std;
 
ostream& SendVectorToOstreamWithSpaces(ostream& os, const vector<string>& v) {
    bool is_first = true;
    for (const string& t : v) {
        if (!is_first) {
            os << " "s;
        }
        os << t;
        is_first = false;
    }
    return os;
}
 
enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};
 
struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};
 
istream& operator>>(istream& is, Query& q) {
    string action;
    q = {};
    is >> action;
    if (action == "NEW_BUS"s) {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count;
        is >> stop_count;
        for (int i = 0; i < stop_count; ++i) {
            string stop;
            is >> stop;
            q.stops.push_back(stop);
        }
    }
    else if (action == "BUSES_FOR_STOP"s) {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    }
    else if (action == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    }
    else if (action == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }
    return is;
}
 
struct BusesForStopResponse {
    vector<string> buses;
};
 
ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if (r.buses.empty()) {
        os << "No stop"s;
        return os;
    }
    else {
        SendVectorToOstreamWithSpaces(os, r.buses);
        return os;
    }
}

struct StopsForBusResponse {
    string target_bus;
    vector<pair<string, vector<string>>> buses_stops;
};
ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if (r.buses_stops.empty()) {
        os << "No bus"s;
        return os;
    }
    bool flag = false;
    for (const auto& [stop_name, vector_buses] : r.buses_stops) {
        if (flag) {
            os << endl;
        }
        flag = true;
        os << "Stop "s << stop_name << ":";
        if (vector_buses.size() == 1) {
            os << " no interchange"s;
            continue;
        }
        for (const string& bus : vector_buses) {
            if (bus == r.target_bus) {
                continue;
            }
            os << " "s << bus;
        }
    }
    return os;
}
 
struct AllBusesResponse {
    map<string, vector<string>> buses_to_stops;
};
 
ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if (r.buses_to_stops.empty()) {
        os << "No buses"s;
    }
    else {
        bool is_first_bus = true;
        for (const auto& [bus, stops] : r.buses_to_stops) {
            if (!is_first_bus) {
                os << endl;
            }
            os << "Bus "s << bus << ": "s;
            SendVectorToOstreamWithSpaces(os, stops);
            is_first_bus = false;
        }
    }
    return os;
}
 
class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        buses_to_stops_[bus] = stops;
        for (const string& stop : stops) {
            stops_to_buses_[stop].push_back(bus);
        }
    }
 
    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse r;
        if (stops_to_buses_.count(stop) == 0) {
            return r;
        }
        r.buses = stops_to_buses_.at(stop);
        return r;
    }
 
    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse result;
        if (buses_to_stops_.empty()) {
            return result;
        }
        if (buses_to_stops_.count(bus) == 0) {
            return result;
        }
        result.target_bus = bus;
        vector<string> stops = buses_to_stops_.at(bus);
        for (const string& stop : stops) {
            vector<string> tmp_list;
            for (const string& bus_from_list : stops_to_buses_.at(stop)) {
                tmp_list.push_back(bus_from_list);
            }
            result.buses_stops.push_back({stop, tmp_list});
        }
        return result;
    }
 
    AllBusesResponse GetAllBuses() const {
        AllBusesResponse r;
        if (buses_to_stops_.empty()) {
            return r;
        }
        r.buses_to_stops = buses_to_stops_;
        return r;
    }
 
private:
    map<string, vector<string>> buses_to_stops_;
    map<string, vector<string>> stops_to_buses_;
};
 
void TestInputOperandForQuery() {
    istringstream new_bus, buses_for_stop, stops_for_bus, all_buses;
    Query q;
    new_bus.str("NEW_BUS Begovaya 3 1905Goda Barrikandaya Krasnopresnenskaya"s);
 
    new_bus >> q;
    assert(q.type == QueryType::NewBus);
    assert(q.bus == "Begovaya"s);
    assert(q.stops.size() == 3);
    assert(q.stops[1] == "Barrikandaya"s);
 
    buses_for_stop.str("BUSES_FOR_STOP Begovaya"s);
    buses_for_stop >> q;
    assert(q.type == QueryType::BusesForStop);
    assert(q.stop == "Begovaya"s);
 
    stops_for_bus.str("STOPS_FOR_BUS 379"s);
    stops_for_bus >> q;
    assert(q.type == QueryType::StopsForBus);
    assert(q.bus == "379"s);
 
    all_buses.str("ALL_BUSES");
    all_buses >> q;
    assert(q.type == QueryType::AllBuses);
 
    cout << "TestInputOperandForQuery successful!"s << endl;
}
 
void TestOutputOperandBusesForStop() {
    ostringstream no_stop, is_stop;
 
    BusesForStopResponse empty_stop;
    
    no_stop << empty_stop;
 
    assert(no_stop.str() == "No stop"s);
    
    BusesForStopResponse some_stop;
    some_stop.buses = {"379"s, "382"s};
    
    is_stop << some_stop;
 
    assert(is_stop.str() == "379 382"s);
    
    cout << "TestOutputOperandBusesForStop successful!"s << endl;
}
 
void TestOutputOperandAllBuses() {
    ostringstream os_empty, os_not_empty;
    AllBusesResponse r_empty, r_not_empty;
 
    r_not_empty.buses_to_stops.emplace("379k"s, vector<string> {"Begovaya"s, "1905"s});
    r_not_empty.buses_to_stops.emplace("345"s, vector<string> {"Barrikadnaya"s, "Shalyapin"s});
 
    os_not_empty << r_not_empty;
    os_empty << r_empty;
 
    assert(os_not_empty.str() == "Bus 345: Barrikadnaya Shalyapin\nBus 379k: Begovaya 1905"s);
    assert(os_empty.str() == "No buses"s);
 
    cout << "TestOutputOperandAllBuses successful!"s << endl;
}
 
int main() {
    int query_count;
    Query q;
 
    cin >> query_count;
    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}