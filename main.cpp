#include <chrono>
#include <future>
#include <iostream>
#include <vector>

class Clicker
{
    public:
    Clicker():
    start_(std::chrono::high_resolution_clock::now())
    {}
    double millisec() const {
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        auto t = high_resolution_clock::now();
        return duration_cast< milliseconds >(t - start_).count();
    }
    private:
    std::chrono::time_point< std::chrono::high_resolution_clock > start_;
};

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

value_t sumV(data_t const* vals, size_t i, size_t end) {
    value_t s{0};
    for (; i < end; ++i) {
        s += (*vals)[i];
    }
    return s;
}

int main() {
    constexpr size_t size{1'000'000'000};
    double init{0}, total{0};
    value_t sum{0};
    data_t values(size, 1);
    int count;
    std::cin >> count;
    size_t work = size / count;
    Clicker cl;
    std::vector<std::future<value_t>> futures;
    futures.reserve(count);
    init = cl.millisec();
    for (int i = 0; i < count - 1; ++i) {
        futures.push_back(std::async(std::launch::async, sumV, &values, i * work, (i + 1) * work));
    }
    futures.push_back(std::async(std::launch::async, sumV, &values, (count - 1) * work, size));
    for (int i = 0; i < count; ++i) {
        sum += futures[i].get();
    }
    total = cl.millisec();
    std::cout << total - init << '\n';
}