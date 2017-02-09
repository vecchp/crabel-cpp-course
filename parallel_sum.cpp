#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <iterator>


//http://codereview.stackexchange.com/questions/45557/summing-values-in-a-vector-using-threads
template <typename Iterator>
typename std::iterator_traits<Iterator>::value_type
adder(Iterator begin, Iterator end)
{
    using T = typename std::iterator_traits<Iterator>::value_type;
    return std::accumulate(begin, end, T());
}

template <typename Iterator>
typename std::iterator_traits<Iterator>::value_type
parallel_sum_1(Iterator begin, Iterator end)
{
    using T = typename std::iterator_traits<Iterator>::value_type;
    auto midpoint = begin + std::distance(begin, end) / 2;
    std::future<T> f1 = std::async(std::launch::async, adder<Iterator>, begin, midpoint);
    std::future<T> f2 = std::async(std::launch::async, adder<Iterator>, midpoint, end);
    return f1.get() + f2.get();
}


template <typename Iterator>
typename std::iterator_traits<Iterator>::value_type
parallel_sum_2(Iterator begin, Iterator end)
{
    using T = typename std::iterator_traits<Iterator>::value_type;
    auto parts = std::thread::hardware_concurrency();

    std::vector<std::future<T>> dispatcher;
    dispatcher.reserve(parts);

    auto dist = std::distance(begin, end);
    auto chunk = dist / parts;
    auto remainder = dist % parts;

    for (size_t i = 0; i < parts - 1; ++i){
        auto next_end = std::next(begin, chunk + (remainder ? 1 : 0));
        dispatcher.emplace_back(std::async(std::launch::async, adder<Iterator>, begin, next_end));
        begin = next_end;
        if (remainder) remainder -= 1;
    }
    // last chunk
    dispatcher.emplace_back(std::async(std::launch::async, adder<Iterator>, begin, end));

    T sum = 0;
    for (auto&& i : dispatcher){
        sum += i.get();
    }

    return sum;
}

int main()
{
    std::vector<long> v;
    for(auto i = 0; i < 10000; ++i) {
        v.push_back(i);
    }
    auto total = parallel_sum_1(v.begin(), v.end());
    auto total_2 = parallel_sum_2(v.begin(), v.end());
    std::cout << total << " " << total_2 << "\n";
}