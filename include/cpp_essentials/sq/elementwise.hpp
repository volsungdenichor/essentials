#ifndef CPP_ESSENTIALS_SQ_ELEMENTWISE_HPP_
#define CPP_ESSENTIALS_SQ_ELEMENTWISE_HPP_

#include <functional>

#include <cpp_essentials/math/arithmetic_functors.hpp>
#include <cpp_essentials/cc/cc.hpp>

#include <cpp_essentials/sq/zip.hpp>
#include <cpp_essentials/sq/map.hpp>

namespace cpp_essentials::sq
{

template
    < class Iter1
    , class Iter2
    , CONCEPT = cc::Add<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator +(const core::iterator_range<Iter1>& lhs, const core::iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, core::plus());
}

template
    < class Iter1
    , class Iter2
    , CONCEPT = cc::Subtract<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator -(const core::iterator_range<Iter1>& lhs, const core::iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, core::minus());
}

template
< class Iter1
    , class Iter2
    , CONCEPT = cc::Multiply<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator *(const core::iterator_range<Iter1>& lhs, const core::iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, core::multiplies());
}

template
    < class Iter1
    , class Iter2
    , CONCEPT = cc::Divide<cc::iter_ref<Iter1>, cc::iter_ref<Iter2>>>
auto operator /(const core::iterator_range<Iter1>& lhs, const core::iterator_range<Iter2>& rhs)
{
    return zip(lhs, rhs, core::divides());
}


template
    < class Iter
    , class T
    , CONCEPT = cc::Add<cc::iter_ref<Iter>, T>>
auto operator +(const core::iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, core::plus(rhs));
}

template
    < class T
    , class Iter
    , CONCEPT = cc::Add<T, cc::iter_ref<Iter>>>
auto operator +(T lhs, const core::iterator_range<Iter>& rhs)
{
    return rhs + lhs;
}


template
    < class Iter
    , class T
    , CONCEPT = cc::Subtract<cc::iter_ref<Iter>, T>>
auto operator -(const core::iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, core::minus(rhs));
}

template
    < class T
    , class Iter
    , CONCEPT = cc::Subtract<T, cc::iter_ref<Iter>>>
auto operator -(T lhs, const core::iterator_range<Iter>& rhs)
{
    return map(rhs, core::minus.bind_left(lhs));
}


template
    < class Iter
    , class T
    , CONCEPT = cc::Multiply<cc::iter_ref<Iter>, T>>
auto operator *(const core::iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, core::multiplies(rhs));
}

template
    < class T
    , class Iter
    , CONCEPT = cc::Multiply<T, cc::iter_ref<Iter>>>
auto operator *(T lhs, const core::iterator_range<Iter>& rhs)
{
    return rhs * lhs;
}


template
    < class Iter
    , class T
    , CONCEPT = cc::Divide<cc::iter_ref<Iter>, T>>
auto operator /(const core::iterator_range<Iter>& lhs, T rhs)
{
    return map(lhs, core::divides(rhs));
}

template
    < class T
    , class Iter
    , CONCEPT = cc::Divide<T, cc::iter_ref<Iter>>>
auto operator /(T lhs, const core::iterator_range<Iter>& rhs)
{
    return map(rhs, core::divides.bind_left(lhs));
}


template <class Iter>
auto operator +(const core::iterator_range<Iter>& item)
{
    return item;
}

template <class Iter>
auto operator -(const core::iterator_range<Iter>& item)
{
    return map(item, core::negate());
}

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_ELEMENTWISE_HPP_ */
