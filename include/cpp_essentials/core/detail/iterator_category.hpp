#ifndef CPP_ESSENTIALS_CORE_DETAIL_ITERATOR_CATEGORY_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_ITERATOR_CATEGORY_HPP_

#pragma once

#include <iterator>

namespace cpp_essentials::core
{

namespace detail
{

template <class Cat>
struct iter_cat_to_int
{
};

template <>
struct iter_cat_to_int<std::input_iterator_tag>
{
    static constexpr int value = 1;
};

template <>
struct iter_cat_to_int<std::forward_iterator_tag>
{
    static constexpr int value = 2;
};

template <>
struct iter_cat_to_int<std::bidirectional_iterator_tag>
{
    static constexpr int value = 3;
};

template <>
struct iter_cat_to_int<std::random_access_iterator_tag>
{
    static constexpr int value = 4;
};

template <int V>
struct int_to_iter_cat
{
};

template <>
struct int_to_iter_cat<1>
{
    using type = std::input_iterator_tag;
};

template <>
struct int_to_iter_cat<2>
{
    using type = std::forward_iterator_tag;
};

template <>
struct int_to_iter_cat<3>
{
    using type = std::bidirectional_iterator_tag;
};

template <>
struct int_to_iter_cat<4>
{
    using type = std::random_access_iterator_tag;
};

template <int V>
using int_to_iter_cat_t = typename int_to_iter_cat<V>::type;

template <class... Cats>
struct _common_iterator_category
{
};

template <class Cat0, class Cat1>
struct _common_iterator_category<Cat0, Cat1>
{
    using type = int_to_iter_cat_t<std::min(iter_cat_to_int<Cat0>::value, iter_cat_to_int<Cat1>::value)>;
};

} /* namespace detail */

template <class... Cats>
using common_iterator_category = typename detail::_common_iterator_category<Cats...>::type;

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_ITERATOR_CATEGORY_HPP */