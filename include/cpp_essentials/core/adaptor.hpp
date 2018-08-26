#ifndef CPP_ESSENTIALS_CORE_ADAPTOR_HPP_
#define CPP_ESSENTIALS_CORE_ADAPTOR_HPP_

#pragma once

namespace cpp_essentials::core
{

template <class Func>
struct adaptor_t
{
    Func func;
};

template <class T, class Func>
auto operator |(T&& item, const adaptor_t<Func>& adaptor) -> decltype(auto)
{
    return adaptor.func(std::forward<T>(item));
}

template <class Func>
auto make_adaptor(Func func) -> adaptor_t<Func>
{
    return { std::move(func) };
}

template <class F1, class F2>
auto operator >>(const adaptor_t<F1>& lhs, const adaptor_t<F2>& rhs)
{
    return make_adaptor([=](auto&& item)
    {
        return item | lhs | rhs;
    });
}


template <class Self>
struct adaptable
{
    using self_type = Self;

    template <class... Args>
    auto operator ()(Args&&... args) const
    {
        return make_adaptor([=](auto&& range) -> decltype(auto)
        {
            return self()(std::move(range), std::move(args)...);
        });
    }

    const self_type& self() const
    {
        return static_cast<const self_type&>(*this);
    }
};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ADAPTOR_HPP_ */