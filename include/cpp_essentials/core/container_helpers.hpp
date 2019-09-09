#ifndef CPP_ESSENTIALS_CORE_CONTAINER_HELPERS_HPP_
#define CPP_ESSENTIALS_CORE_CONTAINER_HELPERS_HPP_

#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::core
{

template
    < class Container
    , class Range
    , CONCEPT = cc::InputRange<Range>>
Container& insert(Container& container, cc::range_iter<Container> iter, Range&& range)
{
    container.insert(iter, std::begin(range), std::end(range));
    return container;
}

template
    < class Container
    , class Range
    , CONCEPT = cc::InputRange<Range>>
Container& push_front(Container& container, Range&& range)
{
    return insert(container, std::begin(container), std::move(range));
}

template
    < class Container
    , class Range
    , CONCEPT = cc::InputRange<Range>>
Container& push_back(Container& container, Range&& range)
{
    return insert(container, std::end(container), std::move(range));
}

template
    < class Container
    , class Range
    , CONCEPT = cc::InputRange<Range>>
void erase(Container& container, Range&& range)
{
    container.erase(std::begin(range), std::end(range));
}

template
    < class Container
    , class Pred
    , CONCEPT = cc::UnaryPredicate<Pred, cc::range_ref<Container>>>
void erase_if(Container& container, Pred&& pred)
{    
    erase(container, remove_if(container, std::move(pred)));
}

namespace detail
{

struct try_at_fn
{
    template <class Container, class Key>
    decltype(auto) operator ()(Container&& container, Key&& key) const
    {
        using ::cpp_essentials::core::map_try_get;
        
        if constexpr (is_associative_container_v<std::decay_t<Container>>)
        {
            return map_try_get(container, key);
        }
        else
        {
            return core::front_or_none(core::views::drop(container, (std::ptrdiff_t)key));
        }
    }
};

struct at_fn
{
    template <class Container, class Key>
    decltype(auto) operator ()(Container&& container, Key&& key) const
    {
        using ::cpp_essentials::core::str;
        
        static constexpr try_at_fn _try_at{};
        return _try_at(container, key).value_or_throw([&]() { return str("missing key '", key, "'"); });
    }
};

} /* namespace detail */

static const auto try_at = adaptable{ detail::try_at_fn{} };
static const auto at = adaptable{ detail::at_fn{} };

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_CONTAINER_HELPERS_HPP_ */
