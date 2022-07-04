#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include "CircularBuffer.hpp"
#include "DataQueue.hpp"

namespace {
    inline void writeToFile(std::ofstream &file, DataQueue::value_type &pair, std::ofstream &speedInfo) {
        const auto start = std::chrono::steady_clock::now();
        file.write(reinterpret_cast<const char*>(pair.first), pair.second);
        const auto time = static_cast<std::chrono::duration<double>>
            ((std::chrono::steady_clock::now() - start)).count();
        speedInfo << pair.second / time / CircularBuffer::bytesInMegabyte << "(MB/s)\n";
    }
}

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    int count;
    if (argc < 2) {
        count = -1;
    } else {
        std::istringstream stream(argv[argc - 1]);
        stream >> count;
        if (stream.fail()) {
            std::cerr << "Error with parsing number\n";
            return 1;
        }
        if (count == 0) {
            std::cerr << "Number of writing must be greater than 0\n";
            return 1;
        }
    }

    const char* filename = "../output.bin";
    std::ofstream output{filename, std::ios_base::binary};
    const std::filesystem::path ex = filename;
    DataQueue dataQueue;
    CircularBuffer circularBuffer(dataQueue.capacity());
    constexpr auto limit = 2048;
    std::mutex mutex;
    std::condition_variable not_full, not_empty;
    const auto start = std::chrono::steady_clock::now();

    std::thread producer{[&]{
        bool unlimited = false;
        if (count == -1) {
            count = 1;
            unlimited = true;
        }
        while (file_size(ex) / CircularBuffer::bytesInMegabyte + dataQueue.size() < limit) {
            const auto start = std::chrono::steady_clock::now();
            for (int i = 0; (i < count) && (static_cast<std::chrono::duration<double>>
                ((std::chrono::steady_clock::now() - start)).count() < 1); ++i) {
                circularBuffer.fill();
                if (dataQueue.full()) {
                    std::unique_lock<std::mutex> uniqueLock{mutex};
                    not_full.wait(uniqueLock, [&] { return !dataQueue.full(); });
                }
                dataQueue.push({circularBuffer.back().get(), CircularBuffer::bytesInMegabyte});
                not_empty.notify_one();
            }
            const auto end = std::chrono::steady_clock::now();
            if (auto temp = static_cast<std::chrono::duration<double>>(end - start).count(); !unlimited &&
                temp < 1) {
                std::this_thread::sleep_for((std::chrono::milliseconds(1000 - long(temp))));
            }
        }
    }};

    std::thread consumer{[&]{
        std::ofstream speedInfo{"../speed_info.txt"};
        while (file_size(ex) / CircularBuffer::bytesInMegabyte < limit) {
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
