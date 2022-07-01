#include "DataQueue.hpp"

#include <queue>
#include <stdexcept>

DataQueue &DataQueue::operator=(DataQueue&& rhs) noexcept {
    if (this != &rhs) {
        m_queue = std::move(rhs.m_queue);
    }
    return *this;
}

DataQueue &DataQueue::operator=(const DataQueue &rhs) {
    if (this != &rhs) {
        m_queue = rhs.m_queue;
    }
    return *this;
}

DataQueue::value_type DataQueue::front() const noexcept {
    return m_queue.front();
}

DataQueue::value_type DataQueue::back() const noexcept {
    return m_queue.back();
}

void DataQueue::push(const value_type& c) {
    if (m_queue.size() >= m_capacity) {
        throw std::runtime_error("DataQueue is full");
    }
    m_queue.push(c);
}

DataQueue::value_type& DataQueue::pop() {
    if (empty()) {
        throw std::runtime_error("DataQueue is empty");
    }
    auto& temp = m_queue.front();
    m_queue.pop();
    return temp;
}

bool DataQueue::empty() const noexcept {
    return m_queue.empty();
}

bool DataQueue::full() const noexcept {
    return m_queue.size() == m_capacity;
}

unsigned int DataQueue::capacity() const noexcept {
    return m_capacity;
}

size_t DataQueue::size() const noexcept {
    return m_queue.size();
}
