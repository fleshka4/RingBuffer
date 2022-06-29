#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <queue>

class RingBuffer {
public:
    using value_type = unsigned int;

    RingBuffer() = default;

    RingBuffer(const RingBuffer&) = default;

    RingBuffer(RingBuffer&&) noexcept = default;

    RingBuffer& operator=(const RingBuffer& rhs);

    RingBuffer& operator=(RingBuffer&& rhs) noexcept;

    [[nodiscard]] value_type front() const noexcept;

    [[nodiscard]] value_type back() const noexcept;

    void push(value_type);

    value_type pop();

    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] bool full() const noexcept;

    [[nodiscard]] value_type capacity() const noexcept;

    [[nodiscard]] size_t size() const noexcept;
private:
    std::queue<value_type> m_queue;
    const unsigned int m_capacity = 256;
};

#endif
