#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <queue>

class DataQueue {
public:
    using value_type = std::pair<unsigned int*, unsigned int>;

    DataQueue() = default;

    DataQueue(const DataQueue&) = default;

    DataQueue(DataQueue&&) noexcept = default;

    DataQueue& operator=(const DataQueue& rhs);

    DataQueue& operator=(DataQueue&& rhs) noexcept;

    [[nodiscard]] value_type front() const noexcept;

    [[nodiscard]] value_type back() const noexcept;

    void push(const value_type&);

    value_type& pop();

    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] bool full() const noexcept;

    [[nodiscard]] unsigned int capacity() const noexcept;

    [[nodiscard]] size_t size() const noexcept;
private:
    std::queue<value_type> m_queue;
    const unsigned int m_capacity = 256;
};

#endif
