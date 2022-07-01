#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>

#include <boost/circular_buffer.hpp>

#include "DataQueue.hpp"

namespace {
    constexpr auto bytesInMegabytes = 1024 * 1024;

    constexpr inline unsigned int countOfDigits(unsigned int n) {
        if (n == 0)
            return 1;
        unsigned int count = 1;
        while (n /= 10)
            ++count;
        return count;
    }

    inline void fill(boost::circular_buffer<DataQueue::value_type> &buffer) {
        static unsigned int counter = 0;
        unsigned int curSize = 0, i = 0;
        unsigned int temp = counter;
        for (; curSize < bytesInMegabytes - countOfDigits(counter) - 1; ++i) {
            curSize += countOfDigits(counter++) + 1;
        }

        auto *ptr = static_cast<unsigned int *>(::malloc(i * sizeof(unsigned int)));
        for (unsigned int j = 0; j < i; ++j) {
            ptr[j] = temp++;
        }
        buffer.push_front({ptr, i});
    }

    inline void writeToFile(std::ofstream &file, DataQueue::value_type &pair, std::ofstream &speedInfo) {
        const auto start = std::chrono::steady_clock::now();
        for (unsigned int i = 0; i < pair.second; ++i) {
            file << pair.first[i] << '\n';
        }
        const auto time = static_cast<std::chrono::duration<double>>
            ((std::chrono::steady_clock::now() - start)).count();
        speedInfo << pair.second / time << "(s.u./s)\n";
        ::free(pair.first);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    const char* filename = "../output.txt";
    std::ofstream output{filename};
    const std::filesystem::path ex = filename;
    DataQueue dataQueue;
    boost::circular_buffer<DataQueue::value_type> buffer(dataQueue.capacity());
    constexpr auto limit = 2048;
    std::mutex mutex;
    std::condition_variable not_full, not_empty;

    std::thread producer{[&]{
        while (file_size(ex) / bytesInMegabytes + dataQueue.size() < limit) {
            fill(buffer);
            if (dataQueue.full()) {
                std::unique_lock<std::mutex> uniqueLock{mutex};
                not_full.wait(uniqueLock, [&]{ return !dataQueue.full(); });
            }
            dataQueue.push(buffer.front());
            not_empty.notify_one();
        }
    }};

    std::thread consumer{[&]{
        std::ofstream speedInfo{"../speed_info.txt"};
        while (file_size(ex) / bytesInMegabytes < limit) {
            if (dataQueue.empty()) {
                std::unique_lock<std::mutex> uniqueLock{mutex};
                not_empty.wait(uniqueLock, [&]{ return !dataQueue.empty(); });
            }
            writeToFile(output, dataQueue.pop(), speedInfo);
            not_full.notify_one();
        }
    }};

    producer.join();
    consumer.join();
}
