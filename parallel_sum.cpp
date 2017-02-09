#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>

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
parallel_sum(Iterator begin, Iterator end)
{
    using T = typename std::iterator_traits<Iterator>::value_type;
    auto midpoint = begin + std::distance(begin, end) / 2;
    std::future<T> f1 = std::async(std::launch::async, adder<Iterator>, begin, midpoint);
    std::future<T> f2 = std::async(std::launch::async, adder<Iterator>, midpoint, end);
    return f1.get() + f2.get();
}

int main()
{
    std::vector<int> v;
    for(int i = 0; i < 100000; ++i) {
        v.push_back(i);
    }
    int total = parallel_sum(v.begin(), v.end());
    std::cout << total << "\n";
}