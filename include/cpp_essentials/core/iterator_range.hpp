#ifndef CPP_ESSENTIALS_CORE_ITERATOR_RANGE_HPP_
#define CPP_ESSENTIALS_CORE_ITERATOR_RANGE_HPP_

#pragma once

#include <iterator>
#include <algorithm>
#include <tuple>

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/concepts/concepts.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Container, class Iter, class = void>
struct is_iterator_constructible : std::false_type {};

template <class Container, class Iter>
struct is_iterator_constructible<
    Container,
    Iter,
    concepts::void_t<
    std::enable_if_t<concepts::iterator<Iter>::value>,
    decltype(Container(std::declval<Iter>(), std::declval<Iter>()))>>
    : std::true_type {};

} /* namespace detail */



template <class Iter, CONCEPT_IF(concepts::InputIterator<Iter>)>
class iterator_range
{
private:
    template <class Container>
    using is_iterator_constructible = detail::is_iterator_constructible<Container, Iter>;

public:
    using iterator = Iter;
    using iterator_category = typename std::iterator_traits<iterator>::iterator_category;
    using value_type = typename std::iterator_traits<iterator>::value_type;
    using reference = typename std::iterator_traits<iterator>::reference;
    using difference_type = typename std::iterator_traits<iterator>::difference_type;
    using size_type = std::make_unsigned_t<difference_type>;

    iterator_range()
        : iterator_range{ iterator{}, iterator{} }
    {
    }

    iterator_range(iterator begin, iterator end)
        : _begin{ begin }
        , _end{ end }
    {
    }

    iterator_range(iterator begin, size_type size)
        : iterator_range{ begin, std::next(begin, size) }
    {
    }

    template <class Range, CONCEPT_IF(concepts::InputRange<Range>)>
    iterator_range(Range&& range)
        : iterator_range{ std::begin(range), std::end(range) }
    {
    }



    auto begin() const -> iterator
    {
        return _begin;
    }

    auto end() const -> iterator
    {
        return _end;
    }



    auto size() const -> size_type
    {
        return static_cast<size_type>(std::distance(begin(), end()));
    }

    auto empty() const -> bool
    {
        return begin() == end();
    }

    explicit operator bool() const
    {
        return !empty();
    }



    auto front() const -> reference
    {
        return *begin();
    }

    auto back() const -> reference
    {
        return *std::prev(end());
    }

    auto operator [](difference_type index) const -> reference
    {
        return *std::next(begin(), index);
    }

    auto at(difference_type index) const -> reference
    {
        return (*this)[index];
    }

    auto operator *() const -> reference
    {
        EXPECTS(!empty());
        return front();
    }

    template <class Container, CONCEPT_IF(is_iterator_constructible<Container>::value)>
    operator Container() const
    {
        return { begin(), end() };
    }

private:
    iterator _begin;
    iterator _end;
};



template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator ==(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return std::equal(std::begin(rhs), std::end(rhs), lhs.begin(), lhs.end());
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator !=(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return !(lhs == rhs);
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator <(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), std::begin(rhs), std::end(rhs));
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator >(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return rhs < lhs;
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator <=(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return !(lhs > rhs);
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator >=(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return !(lhs < rhs);
}

#if 0
template <class Range, class Iter, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator ==(Range&& lhs, const iterator_range<Iter>& rhs) -> bool
{
    return rhs == lhs;
}

template <class Range, class Iter, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator !=(Range&& lhs, const iterator_range<Iter>& rhs) -> bool
{
    return rhs != lhs;
}
#endif




template
    < class Iter
    , CONCEPT_IF(concepts::InputIterator<Iter>)>
auto make_range(Iter begin, Iter end) -> iterator_range<Iter>
{
    return iterator_range<Iter>{ begin, end };
}

template
< class Iter
    , CONCEPT_IF(concepts::InputIterator<Iter>)>
auto make_range(const std::pair<Iter, Iter>& pair) -> iterator_range<Iter>
{
    return make_range(std::get<0>(pair), std::get<1>(pair));
}

template
< class Range
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto make_range(Range&& range)
{
    return make_range(std::begin(range), std::end(range));
}

} /* cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ITERATOR_RANGE_HPP_ */
