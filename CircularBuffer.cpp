#include "CircularBuffer.hpp"

#include <memory>

CircularBuffer::CircularBuffer(size_t capacity):
    m_ptr(std::make_unique<pointer_type[]>(capacity)),
    m_capacity(capacity)
{
    for (uint32_t i = 0; i < capacity; ++i) {
        m_ptr[i] = std::make_unique<value_type[]>(bytesInMegabyte);
    }
}

void CircularBuffer::reset() {
    m_head = m_tail;
    m_full = false;
}

bool CircularBuffer::empty() const noexcept {
    return (!m_full && (m_head == m_tail));
}

bool CircularBuffer::full() const noexcept {
    return m_full;
}

size_t CircularBuffer::capacity() const noexcept {
    return m_capacity;
}

size_t CircularBuffer::size() const noexcept {
    size_t size = m_capacity;

    if (!m_full) {
        if (m_head >= m_tail) {
            size = m_head - m_tail;
        } else {
            size = m_capacity + m_head - m_tail;
        }
    }

    return size;
}

void CircularBuffer::fill() {
    for (uint32_t i = 0; i < bytesInMegabyte; ++i) {
        m_ptr[m_head][i] = counter++;
    }
    if (m_full)
    {
        m_tail = (m_tail + 1) % m_capacity;
    }

    m_head = (m_head + 1) % m_capacity;

    m_full = m_head == m_tail;
}

const CircularBuffer::pointer_type& CircularBuffer::front() const noexcept {
    return m_ptr[m_tail];
}

const CircularBuffer::pointer_type& CircularBuffer::back() const noexcept {
    return m_ptr[m_head];
}
