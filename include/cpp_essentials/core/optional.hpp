#ifndef CPP_ESSENTIALS_CORE_OPTIONAL_HPP_
#define CPP_ESSENTIALS_CORE_OPTIONAL_HPP_

#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>

#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::core
{

struct none_t
{
};

static constexpr none_t none;

template <class T>
class optional;

class optional_access_error : public std::runtime_error
{
public:
    optional_access_error()
        : std::runtime_error{ "optional_access_error" }
    {
    }
};



namespace detail
{

struct value_tag { };
struct optional_tag { };

template <class T>
struct is_optional : std::false_type {};

template <class T>
struct is_optional<optional<T>> : std::true_type {};

template <class Adaptee>
class optional_base
{
private:
    using self_type = Adaptee;

public:
    template <class Func>
    auto then(Func func) const
    {
        using type = decltype(func(self().value()));

        using result_type = std::conditional_t
            < is_optional<type>::value
            , type
            , optional<type>>;

        return self().has_value() ? result_type{ func(self().value()) } : result_type{};
    }

    template <class T>
    decltype(auto) value_or(const T& default_value) const
    {
        return self().has_value() ? self().value() : default_value;
    }

    template <class Func>
    decltype(auto) value_or_eval(Func func) const
    {
        return self().has_value() ? self().value() : func();
    }
   
    decltype(auto) value_or_default() const
    {
        using type = std::remove_reference_t<decltype(self().value())>;
        return value_or(type{});
    }

    template
        < class Exception
        , CONCEPT = cc::BaseOf<std::exception, Exception>>
    decltype(auto) value_or_throw(const Exception& exception) const
    {
        if (!self().has_value())
        {
            throw exception;
        }
        return self().value();
    }

    decltype(auto) value_or_throw(const std::string& message) const
    {
        return value_or_throw(std::runtime_error{ message });
    }

    template <class T>
    auto and_optional(const optional<T>& other) const
    {
        return self().has_value() ? other : optional<T>{};
    }

    template <class T>
    auto or_optional(const optional<T>& other) const
    {
        return self().has_value() ? self() : other;
    }

    bool operator !() const
    {
        return !self().has_value();
    }

    explicit operator bool() const
    {
        return self().has_value();
    }

private:
    const self_type& self() const
    {
        return static_cast<const self_type&>(*this);
    }
};

} /* namespace detail */

template <class T>
class optional : public detail::optional_base<optional<T>>
{
public:
    using value_type = T;

    using const_reference = const T&;
    using reference = T& ;

    using const_pointer = const T*;
    using pointer = T*;

    static_assert(!std::is_rvalue_reference_v<T>, "rvalue reference is not allowed");

    optional()
        : _opt{}
    {
    }

    optional(none_t)
        : optional{}
    {
    }

    optional(const value_type& value)
        : _opt{ value }
    {
    }

    optional(value_type&& value)
        : _opt{ std::move(value) }
    {
    }

    optional(const optional& other)
        : _opt{ other._opt }
    {
    }

    optional(optional&& other)
        : _opt{ std::move(other._opt) }
    {
    }

    template <class U>
    optional(const optional<U>& other)
        : _opt{ static_cast<std::optional<U>>(other) }
    {
    }

#if 1
    optional(const std::optional<value_type>& other)
        : _opt{ other }
    {
    }

    optional(std::optional<value_type>&& other)
        : _opt{ std::move(other) }
    {
    }
#endif

    ~optional()
    {
    }

    optional& operator =(optional other)
    {
        swap(other);
        return *this;
    }

    operator std::optional<T>() const
    {
        return _opt;
    }

    bool has_value() const
    {
        return _opt.has_value();
    }

    const_reference operator *() const
    {
        return *get();
    }

    reference operator *()
    {
        return *get();
    }

    const_pointer operator ->() const
    {
        return get();
    }

    pointer operator ->()
    {
        return get();
    }

    const_reference value() const
    {
        return *get();
    }

    reference value()
    {
        return *get();
    }

    const_pointer get() const
    {
        check_not_null();
        return &(*_opt);
;
    }

    pointer get()
    {
        check_not_null();
        return &(*_opt);
    }

    void swap(optional& other)
    {
        std::swap(_opt, other._opt);
    }

private:
    void check_not_null() const
    {
        if (!has_value())
        {
            throw optional_access_error{};
        }
    }

private:
    std::optional<value_type> _opt;
};

template <class T>
class optional<T&> : public detail::optional_base<optional<T&>>
{
public:
    using value_type = std::remove_const_t<T>;
    using reference = T & ;
    using pointer = T * ;

    optional()
        : _ptr{}
    {
    }

    optional(none_t)
        : optional{}
    {
    }

    optional(reference value)
        : _ptr{ &value }
    {
    }

    optional(const optional& other)
        : _ptr{ other._ptr }
    {
    }

    optional(value_type&&) = delete;

    optional(optional&& other)
        : optional{}
    {
        std::swap(_ptr, other._ptr);
    }

    optional& operator =(optional other)
    {
        swap(other);
        return *this;
    }

    bool has_value() const
    {
        return _ptr;
    }

    reference operator *() const
    {
        return *get();
    }

    pointer operator ->() const
    {
        return get();
    }

    reference value() const
    {
        return *get();
    }

    pointer get() const
    {
        check_not_null();
        return _ptr;
    }

    void swap(optional& other)
    {
        std::swap(_ptr, other._ptr);
    }

private:
    void check_not_null() const
    {
        if (!has_value())
        {
            throw optional_access_error{};
        }
    }

private:
    pointer _ptr;
};


template <class T>
struct underlying_type
{
    using type = T;
};

template <class T>
struct underlying_type<optional<T>>
{
    using type = T;
};

template <class T>
struct underlying_type<std::optional<T>>
{
    using type = T;
};

template <class T>
struct is_optional : std::false_type {};

template <class T>
struct is_optional<optional<T>> : std::true_type {};

template <class T>
struct is_optional<std::optional<T>> : std::true_type {};

template <class T>
using underlying_type_t = typename underlying_type<T>::type;

template <class T>
static constexpr bool is_optional_v = is_optional<T>::value;


template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator ==(const optional<T>& lhs, const optional<U>& rhs)
{
    return (!lhs && !rhs) || (lhs && rhs && *lhs == *rhs);
}

template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator !=(const optional<T>& lhs, const optional<U>& rhs)
{
    return !(lhs == rhs);
}



template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator ==(const optional<T>& lhs, const U& rhs)
{
    return lhs && *lhs == rhs;
}

template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator !=(const optional<T>& lhs, const U& rhs)
{
    return !(lhs == rhs);
}



template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator ==(const T& lhs, const optional<U>& rhs)
{
    return rhs == lhs;
}

template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator !=(const T& lhs, const optional<U>& rhs)
{
    return !(lhs == rhs);
}



template <class T>
bool operator ==(const optional<T>& lhs, none_t)
{
    return !lhs;
}

template <class T>
bool operator !=(const optional<T>& lhs, none_t)
{
    return lhs.has_value();
}



template <class T>
bool operator ==(none_t, const optional<T>& rhs)
{
    return !rhs.has_value();
}

template <class T>
bool operator !=(none_t, const optional<T>& rhs)
{
    return rhs.has_value();
}

namespace detail
{

struct make_optional_fn
{
    template <class T>
    auto operator ()(T value) const
    {
        return optional<T> { std::move(value) };
    }

    template <class T>
    auto operator ()(bool condition, T value) const
    {
        return condition ? optional<T> { std::move(value) } : optional<T>{};
    }
};

struct eval_optional_fn
{
    template <class Func>
    auto operator ()(bool condition, Func func) const
    {
        using type = decltype(func());

        return condition ? optional<type> { func() } : optional<type>{};
    }
};

} /* namespace detail */

static constexpr detail::make_optional_fn make_optional = {};
static constexpr detail::eval_optional_fn eval_optional = {};



template <class T>
std::ostream& operator <<(std::ostream& os, const optional<T>& item)
{
    if (item)
    {
        os << *item;
    }
    else
    {
        os << "{none}";
    }

    return os;
}

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_OPTIONAL_HPP_ */
