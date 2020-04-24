
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <thread>
#include <utility>
#include <vector>

template <typename Number>
void selection_sort(std::vector<Number>& values) {
    const auto end = values.end();
    for (auto it = values.begin(); it != end; ++it) {
        std::iter_swap(it, std::min_element(it, end));
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

template <typename Number>
void insertion_sort(std::vector<Number>& values) {
    const auto begin = values.begin();
    const auto end = values.end();
    for (auto it = begin; it != end; ++it) {
        const auto place = std::upper_bound(begin, it, *it);  
        std::rotate(place, it, it + 1);
    }
}

template <typename Number>
void optimistic_insertion_sort(std::vector<Number>& values) {
    const auto begin = values.begin();
    const auto end = values.end();
    const auto rend = values.rend();
    for (auto it = begin; it != end; ++it) {
        // Here's the optimistic part: use `find_if` instead of `lower_bound`.
        const auto place = std::find_if(
            std::make_reverse_iterator(it),
            rend,
            [&](const Number& num) {
                return !(*it < num);  // closest num <= *it 
            }).base();

        std::rotate(place, it, it + 1);
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

    std::uint64_t selection_ns;
    std::uint64_t bubble_ns;
    std::uint64_t insertion_ns;
    std::uint64_t optimistic_insertion_ns;

    std::thread selection_thread([&]() {
        selection_ns = bench_generic_sort<std::uniform_int_distribution<int>>(
            &selection_sort<int>,
            size,
            num_iterations);
    });

    std::thread bubble_thread([&]() {
        bubble_ns = bench_generic_sort<std::uniform_int_distribution<int>>(
            &bubble_sort<int>,
            size,
            num_iterations);
    });

    std::thread insertion_thread([&]() {
        insertion_ns = bench_generic_sort<std::uniform_int_distribution<int>>(
            &insertion_sort<int>,
            size,
            num_iterations);
    });

    std::thread optimistic_insertion_thread([&]() {
        optimistic_insertion_ns = bench_generic_sort<std::uniform_int_distribution<int>>(
            &optimistic_insertion_sort<int>,
            size,
            num_iterations);
    });

    selection_thread.join();
    bubble_thread.join();
    insertion_thread.join();
    optimistic_insertion_thread.join();

    const double scale = 1.0 / num_iterations;

    std::cout << "selection: " << selection_ns << " " << (selection_ns * scale) << "\n";
    std::cout << "bubble: " << bubble_ns << " " << (bubble_ns * scale) << "\n";
    std::cout << "insertion: " << insertion_ns << " " << (insertion_ns * scale) << "\n";
    std::cout << "optimistic_insertion: " << optimistic_insertion_ns << " " << (optimistic_insertion_ns * scale) << "\n";
}
