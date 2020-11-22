#pragma once

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>

namespace millrind::core
{

template <class T>
class numeric_iterator : public iterator_facade<numeric_iterator<T>>
{
public:
    numeric_iterator() = default;

    numeric_iterator(T value)
        : _value{ std::move(value) }
    {
    }

    numeric_iterator(const numeric_iterator&) = default;

    numeric_iterator(numeric_iterator&&) = default;

    numeric_iterator& operator =(numeric_iterator other)
    {
        std::swap(_value, other._value);
        return *this;
    }

    auto deref() const
    {
        return _value;
    }

    void advance(std::ptrdiff_t offset)
    {
        _value += offset;
    }

    auto distance_to(const numeric_iterator& other) const
    {
        return other._value - _value;
    }

private:
    T _value;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::numeric_iterator)