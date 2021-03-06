#ifndef CPP_ESSENTIALS_CORE_NUMERIC_HPP_
#define CPP_ESSENTIALS_CORE_NUMERIC_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/numeric_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct range_fn
{   
    template <class T>
    auto operator ()(T first, T last) const
    {
        using step_type = decltype(last - first);
        const auto step = first < last ? step_type(+1) : step_type(-1);

        return make_range(
            numeric_iterator{ first, step },
            numeric_iterator{ last, step });
    }

    template <class T>
    auto operator ()(T last) const
    {
        return (*this)(T{}, last);
    }
};

struct inclusive_range_fn
{
    template <class T>
    auto operator ()(T first, T last) const
    {
        using step_type = decltype(last - first);
        const auto step = first < last ? step_type(+1) : step_type(-1);

        return make_range(
            numeric_iterator{ first, step },
            numeric_iterator{ T(last + step), step });
    }

    template <class T>
    auto operator ()(T last) const
    {
        return (*this)(T{}, last);
    }
};

struct infinite_range_fn
{
    template <class T = int>
    auto operator ()(T start = {}) const
    {
        return make_range(
            infinite_numeric_iterator<T>{ start },
            infinite_numeric_iterator<T>{ });
    }
};

} /* namespace detail */

static constexpr auto range = detail::range_fn{};
static constexpr auto inclusive_range = detail::inclusive_range_fn{};
static constexpr auto infinite_range = detail::infinite_range_fn{};
static constexpr auto ints = detail::infinite_range_fn{};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_NUMERIC_HPP_ */
