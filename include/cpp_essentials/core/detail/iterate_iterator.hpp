#ifndef CPP_ESSENTIALS_CORE_DETAIL_ITERATE_ITERATOR_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_ITERATE_ITERATOR_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter>
class iterate_iterator
    : public core::iterator_facade
    < iterate_iterator<Iter>
    , cc::iter_cat<Iter>
    , Iter>
{
public:
    using base_type = core::iterator_facade
        < iterate_iterator<Iter>
        , cc::iter_cat<Iter>
        , Iter>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    iterate_iterator() = default;

    iterate_iterator(Iter iter)
        : _iter{ iter }
    {
    }

    reference ref() const
    {
        return _iter;
    }

    void inc()
    {
        ++_iter;
    }

    void dec()
    {
        --_iter;
    }

    void advance(difference_type offset)
    {
        std::advance(_iter, offset);
    }

    bool is_equal(const iterate_iterator& other) const
    {
        return _iter == other._iter;
    }

    bool is_less(const iterate_iterator& other) const
    {
        return _iter < other._iter;
    }

    difference_type distance(const iterate_iterator& other) const
    {
        return std::distance(_iter, other._iter);
    }

private:
    Iter _iter;
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_ITERATE_ITERATOR_HPP_ */
