#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <thread>

#include "RingBuffer.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    RingBuffer ringBuffer;
    unsigned int i = 0;
    constexpr auto limit = std::numeric_limits<unsigned int>::max();
    std::mutex mutex;
    std::condition_variable not_full, not_empty;

    const auto start = std::chrono::steady_clock::now();
    std::thread producer{[&]{
        while (i <= limit) {
            try {
                ringBuffer.push(i);
                ++i;
            } catch (const std::runtime_error& e) {
                std::unique_lock<std::mutex> uniqueLock{mutex};
                not_full.wait(uniqueLock, [&]{ return !ringBuffer.full(); });
            }
            not_empty.notify_one();
        }
    }};

    std::thread consumer{[&]{
        std::ofstream timePoints{"../time.txt"};
        std::ofstream output{"../output.txt"};
        while ((!ringBuffer.empty()) || (i - 1 != limit)) {
            try {
                const auto temp = ringBuffer.pop();
                output << temp << '\n';
                timePoints << ringBuffer.size() << ' ' << static_cast<std::chrono::duration<double>>
                    ((std::chrono::steady_clock::now() - start)).count() << "s\n";
            } catch (const std::runtime_error& e) {
                std::unique_lock<std::mutex> uniqueLock{mutex};
                not_empty.wait(uniqueLock, [&]{ return !ringBuffer.empty(); });
            }
            not_full.notify_one();
        }
    }};

    producer.join();
    consumer.join();

    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\nAverage speed: "
              << limit / elapsed_seconds.count() << "(s.u./s)";
}
