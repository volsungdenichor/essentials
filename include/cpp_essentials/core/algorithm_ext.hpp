#ifndef CPP_ESSENTIALS_CORE_ALGORITHM_EXT_HPP_
#define CPP_ESSENTIALS_CORE_ALGORITHM_EXT_HPP_

#pragma once

#include <algorithm>
#include <numeric>

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/return_policy.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class Iter, class OutputIter, class UnaryPred>
Iter copy_while(Iter first, Iter last, OutputIter output, UnaryPred pred)
{
    while (first != last && pred(*first))
    {
        *output = *first;
        ++output;
        ++first;
    }
    return first;
}

struct front_t : adaptable<front_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        EXPECTS(b != e);
        return *b;
    }
};

struct front_or_t : adaptable<front_or_t>
{
    using adaptable::operator();

    template
        < class Range
        , class T
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, T default_value) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return b != e ? *b : default_value;
    }
};

struct front_or_default_t : adaptable<front_or_default_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return b != e ? *b : cc::range_val<Range>{};
    }
};

struct front_or_eval_t : adaptable<front_or_eval_t>
{
    using adaptable::operator();

    template
        < class Range
        , class Func
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::NullaryFunction<Func>>
    auto operator ()(Range&& range, Func func) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return b != e ? *b : func();
    }
};

struct front_or_none_t : adaptable<front_or_none_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const -> decltype(auto)
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return eval_optional(b != e, [&]() -> decltype(auto) { return *b; });
    }
};

struct size_t : adaptable<size_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        return std::distance(std::begin(range), std::end(range));
    }
};

struct empty_t : adaptable<empty_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        return std::begin(range) == std::end(range);
    }
};

struct copy_while_t : adaptable<copy_while_t>
{
    using adaptable::operator();

    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::OutputIterator<OutputIter>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred) const
    {
        return copy_while(std::begin(range), std::end(range), output, std::move(pred));
    }
};

struct copy_until_t : adaptable<copy_until_t>
{
    using adaptable::operator();

    template
        < class Range
        , class OutputIter
        , class UnaryPred
        , CONCEPT = cc::InputRange<Range>
        , CONCEPT = cc::OutputIterator<OutputIter>
        , CONCEPT = cc::UnaryPredicate<UnaryPred, cc::range_ref<Range>>>
    auto operator ()(Range&& range, OutputIter output, UnaryPred&& pred) const
    {
        return copy_while(std::begin(range), std::end(range), output, logical_negation(pred));
    }
};

struct starts_with_t : adaptable<starts_with_t>
{
    using adaptable::operator();

    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT = cc::InputRange<Range1>
        , CONCEPT = cc::InputRange<Range2>
        , CONCEPT = cc::BinaryPredicate<BinaryPred, cc::range_ref<Range1>, cc::range_ref<Range2>>>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto b1 = std::begin(range1);
        auto e1 = std::end(range1);

        return std::search(b1, e1, std::begin(range2), std::end(range2), std::move(pred)) == b1;
    }
};

struct ends_with_t : adaptable<ends_with_t>
{
    using adaptable::operator();

    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT = cc::InputRange<Range1>
        , CONCEPT = cc::InputRange<Range2>
        , CONCEPT = cc::BinaryPredicate<BinaryPred, cc::range_ref<Range1>, cc::range_ref<Range2>>>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto b1 = std::begin(range1);
        auto e1 = std::end(range1);

        auto b2 = std::begin(range2);
        auto e2 = std::end(range2);

        auto size1 = std::distance(b1, e1);
        auto size2 = std::distance(b2, e2);

        return size1 >= size2 && std::equal(b2, e2, std::next(b1, size1 - size2), std::move(pred));
    }
};

struct contains_t : adaptable<contains_t>
{
    using adaptable::operator();

    template
        < class Range1
        , class Range2
        , class BinaryPred = std::equal_to<>
        , CONCEPT = cc::InputRange<Range1>
        , CONCEPT = cc::InputRange<Range2>
        , CONCEPT = cc::BinaryPredicate<BinaryPred, cc::range_ref<Range1>, cc::range_ref<Range2>>>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryPred&& pred = {}) const
    {
        auto b1 = std::begin(range1);
        auto e1 = std::end(range1);

        return std::search(b1, e1, std::begin(range2), std::end(range2), std::move(pred)) != e1;
    }
};

} /* namespace detail */

static constexpr detail::front_t front = {};
static constexpr detail::front_or_t front_or = {};
static constexpr detail::front_or_default_t front_or_default = {};
static constexpr detail::front_or_eval_t front_or_eval = {};
static constexpr detail::front_or_none_t front_or_none = {};
static constexpr detail::size_t size = {};
static constexpr detail::empty_t empty = {};
static constexpr detail::copy_while_t copy_while = {};
static constexpr detail::copy_until_t copy_until = {};
static constexpr detail::starts_with_t starts_with = {};
static constexpr detail::ends_with_t ends_with = {};
static constexpr detail::contains_t contains = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ALGORITHM_EXT_HPP_ */

