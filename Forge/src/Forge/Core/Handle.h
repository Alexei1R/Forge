
#ifndef HANDLE_H
#define HANDLE_H

#include <cstdint>
#include <functional>

class Handle {
public:
    Handle()
        : m_Value(0) {}
    explicit Handle(uint32_t value)
        : m_Value(value) {}

    bool operator==(const Handle& other) const {
        return m_Value == other.m_Value;
    }
    bool operator!=(const Handle& other) const {
        return m_Value != other.m_Value;
    }
    bool operator<(const Handle& other) const {
        return m_Value < other.m_Value;
    }

    uint32_t GetValue() const {
        return m_Value;
    }

private:
    uint32_t m_Value;
};

// Specialization for std::hash within the std namespace
namespace std {
template <>
struct hash<Handle> {
    std::size_t operator()(const Handle& handle) const noexcept {
        return std::hash<uint32_t>()(handle.GetValue());
    }
};
} // namespace std

#endif // HANDLE_H
