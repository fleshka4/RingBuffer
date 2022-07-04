#ifndef CIRCULARBUFFER_HPP
#define CIRCULARBUFFER_HPP

#include <memory>

class CircularBuffer {
public:
    using value_type = uint32_t;

    using pointer_type = std::unique_ptr<value_type[]>;

    static constexpr auto bytesInMegabyte = 1024 * 1024;

    CircularBuffer() = delete;

    explicit CircularBuffer(size_t capacity);

    void reset();

    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] bool full() const noexcept;

    [[nodiscard]] size_t capacity() const noexcept;

    [[nodiscard]] size_t size() const noexcept;

    void fill();

    [[nodiscard]] const pointer_type& front() const noexcept;

    [[nodiscard]] const pointer_type& back() const noexcept;

private:
    std::unique_ptr<pointer_type[]> m_ptr;
    uint32_t counter = 0;
    size_t m_head = 0;
    size_t m_tail = 0;
    const size_t m_capacity;
    bool m_full = false;
};

#endif
