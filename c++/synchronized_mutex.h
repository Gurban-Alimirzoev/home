#include <mutex>
 
using namespace std;
 
template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T())
        : value_(move(initial)) {
    }
 
    struct Access {
        T& ref_to_value;
        lock_guard<mutex> guard;
    };
 
    Access GetAccess() {
        return {value_, lock_guard(m_)};
    }
 
private:
    T value_;
    mutex m_;
};