#include "RingBuffer.hpp"

#include <queue>
#include <stdexcept>

RingBuffer &RingBuffer::operator=(RingBuffer&& rhs) noexcept {
    if (this != &rhs) {
        m_queue = std::move(rhs.m_queue);
    }
    return *this;
}

RingBuffer &RingBuffer::operator=(const RingBuffer &rhs) {
    if (this != &rhs) {
        m_queue = rhs.m_queue;
    }
    return *this;
}

RingBuffer::value_type RingBuffer::front() const noexcept {
    return m_queue.front();
}

RingBuffer::value_type RingBuffer::back() const noexcept {
    return m_queue.back();
}

void RingBuffer::push(value_type c) {
    if (m_queue.size() >= m_capacity) {
        throw std::runtime_error("RingBuffer is full");
    }
    m_queue.push(c);
}

RingBuffer::value_type RingBuffer::pop() {
    if (empty()) {
        throw std::runtime_error("RingBuffer is empty");
    }
    const auto temp = m_queue.front();
    m_queue.pop();
    return temp;
}

bool RingBuffer::empty() const noexcept {
    return m_queue.empty();
}

bool RingBuffer::full() const noexcept {
    return m_queue.size() == m_capacity;
}

RingBuffer::value_type RingBuffer::capacity() const noexcept {
    return m_capacity;
}

size_t RingBuffer::size() const noexcept {
    return m_queue.size();
}
