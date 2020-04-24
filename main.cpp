
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <utility>
#include <vector>

template <typename Number>
void insertion_sort(std::vector<Number>& values) {
    const std::size_t n = values.size();
    for (std::size_t i = 0; i < n; ++i) {
        // `k` is the index of the minimum element in `values[i:n]`
        std::size_t k = i;
        for (std::size_t j = i + 1; j < n; ++j) {
            if (values[j] < values[k]) {
                k = j;
            }
        }
        std::swap(values[i], values[k]);
    }
}

template <typename Number>
void bubble_sort(std::vector<Number>& values) {
    const std::size_t n = values.size();
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i + 1; j < n; ++j) {
            if (values[j] < values[i]) {
                std::swap(values[i], values[j]);
            }
        }
    }
}

template <typename Func>
std::uint64_t time_ns(Func&& func) {
    std::chrono::steady_clock clock;
    const auto before = clock.now();
    func();
    const auto after = clock.now();

    return (after - before) / std::chrono::nanoseconds(1);
}

template <typename Distribution, typename Number>
auto bench_generic_sort(void (*sort)(std::vector<Number>&),
                        std::size_t size,
                        std::size_t num_iterations) {
    std::vector<Number> values(size);
    std::uint64_t total_ns = 0;
    std::mt19937_64 generator;
    const Number min = std::numeric_limits<Number>::min();
    const Number max = std::numeric_limits<Number>::max();
    Distribution distribution(min, max);
    const auto rand = [&]() { return distribution(generator); };

    for (std::size_t i = 0; i < num_iterations; ++i) {
        std::generate(values.begin(), values.end(), rand);
        total_ns += time_ns([&]() { sort(values); });
    }

    return total_ns;
}

int main() {
    const std::size_t size = SIZE;
    const std::size_t num_iterations = NUM_ITERATIONS;

    std::cout << "insertion: " <<  bench_generic_sort<std::uniform_int_distribution<int>>(
        &insertion_sort<int>,
        size,
        num_iterations) << "\n";

    std::cout << "bubble: " << bench_generic_sort<std::uniform_int_distribution<int>>(
        &bubble_sort<int>,
        size,
        num_iterations) << "\n";
}
