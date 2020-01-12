#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>
#include <atomic>

#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/core/string_views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/core/string_functors.hpp>
#include <string_view>
#include <cpp_essentials/meta/serialization_json.hpp>
#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/morphological_operations.hpp>
#include <cpp_essentials/gx/kernels.hpp>
#include <cpp_essentials/gx/lookup_table.hpp>
#include <cpp_essentials/gx/drawing_context.hpp>
#include <cpp_essentials/gx/filters.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <complex>
#include <cpp_essentials/geo/intersection.hpp>
#include <cpp_essentials/geo/projection.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/coordinates_conversion.hpp>
#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/core/expected.hpp>
#include <cpp_essentials/graphs/algorithm.hpp>
#include <cpp_essentials/graphs/tree.hpp>
#include <cpp_essentials/gx/filters.hpp>
#include <cpp_essentials/core/optional_helpers.hpp>
#include <cpp_essentials/core/string_functors.hpp>
#include <cpp_essentials/core/any_range.hpp>

#include <cpp_essentials/kine/animation.hpp>
#include <cpp_essentials/geo/orientation.hpp>
#include <cpp_essentials/geo/clamp.hpp>
#include <cpp_essentials/core/generator.hpp>
#include <cpp_essentials/geo/dcel.hpp>
#include <cpp_essentials/core/program_args.hpp>
#include <cpp_essentials/core/match.hpp>

#include <cpp_essentials/arrays/array.hpp>
#include <cpp_essentials/arrays/transformations.hpp>
#include <cpp_essentials/geo/triangulation.hpp>
#include <cpp_essentials/core/debug_utils.hpp>
#include <cpp_essentials/core/channel.hpp>
#include <cpp_essentials/core/task_queue.hpp>
#include <cpp_essentials/core/event_aggregator.hpp>

#include <cpp_essentials/carto/coords_array.hpp>
#include <cpp_essentials/math/binomial.hpp>

#include <cpp_essentials/fmt/format.hpp>

#include <cpp_essentials/math/regression.hpp>
#include <cpp_essentials/chrono/calendars.hpp>

using namespace cpp_essentials;

using Triple = std::tuple<int, int, int>;

constexpr bool coprime(int a, int b)
{
    return std::gcd(a, b) == 1;
}

auto pythagorean_triples() -> core::iterable<Triple>
{
    using namespace sq;
    return ints(1)
        | flat_map([](int z)
    {
        return range(1, z + 1)
            | take_if([z](int x) { return coprime(x, z); })
            | flat_map([z](int x)
        {
            return range(x, z + 1)
                | filter_map([x, z](int y) -> core::optional<Triple>
            {
                using math::sqr;
                if (sqr(x) + sqr(y) == sqr(z))
                    return Triple{ x, y, z };
                else
                    return core::none;
            });
        });
    });
}

template <class Pred>
struct predicate_t
{
    Pred pred;

    template <class T>
    constexpr bool operator ()(const T& value) const
    {
        if constexpr (std::is_invocable_v<Pred, T>)
        {
            return pred(value);
        }
        else
        {
            return value == pred;
        }
    }
};

template <class Pred>
constexpr predicate_t<Pred> make_predicate(Pred pred)
{
    return { std::move(pred) };
}

template <class Pred>
constexpr predicate_t<Pred> make_predicate(predicate_t<Pred> pred)
{
    return std::move(pred);
}

constexpr auto make_placeholder()
{
    return make_predicate([](const auto&) { return true; });
}

template <class P>
constexpr auto operator !(predicate_t<P> item)
{
    return make_predicate([=](const auto& value)
    {
        return !item(value);
    });
}

template <class L, class R>
constexpr auto operator &&(predicate_t<L> lhs, predicate_t<R> rhs)
{
    return make_predicate([=](const auto& value)
    {
        return lhs(value) && rhs(value);
    });
}

template <class L, class R>
constexpr auto operator ||(predicate_t<L> lhs, predicate_t<R> rhs)
{
    return make_predicate([=](const auto& value)
    {
        return lhs(value) || rhs(value);
    });
}

template <class P, class T>
constexpr auto operator ==(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::equal_to(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator !=(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::not_equal_to(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator <(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::less(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator >(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::greater(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator <=(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::less_equal(std::move(rhs)));
}

template <class P, class T>
constexpr auto operator >=(predicate_t<P> lhs, T rhs)
{
    return std::move(lhs) && make_predicate(core::greater_equal(std::move(rhs)));
}

template <class T, class P>
constexpr auto operator ==(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) == std::move(lhs);
}

template <class T, class P>
constexpr auto operator !=(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) != std::move(lhs);
}

template <class T, class P>
constexpr auto operator <(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) > std::move(lhs);
}

template <class T, class P>
constexpr auto operator >(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) < std::move(lhs);
}

template <class T, class P>
constexpr auto operator <=(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) >= std::move(lhs);
}

template <class T, class P>
constexpr auto operator >=(T lhs, predicate_t<P> rhs)
{
    return std::move(rhs) <= std::move(lhs);
}

template <class T>
constexpr auto between(T lo, T hi)
{
    return make_predicate([=](const auto& value)
    {
        return lo <= value && value < hi;
    });
}

template <class P>
constexpr auto some(predicate_t<P> pred)
{
    return make_predicate([=](const auto& value)
    {
        return value && pred(*value);
    });
}

constexpr auto none()
{
    return make_predicate([](const auto& value)
    {
        return !value;
    });
}

constexpr auto some()
{
    return !none();
}

template <class T>
constexpr auto of()
{
    return make_predicate([](const auto& value)
    {
        return std::holds_alternative<T>(value);
    });
}

template <class T, class P>
constexpr auto of(predicate_t<P> pred)
{
    return make_predicate([=](const auto& value)
    {
        return std::holds_alternative<T>(value) && pred(std::get<T>(value));
    });
}

template <class P, class T, size_t... I>
constexpr bool check(const P& pred, const T& value, std::index_sequence<I...>)
{
    return (std::get<I>(pred)(std::get<I>(value)) && ...);
}

template <class... P>
constexpr auto unpack(P... predicates)
{
    auto p = std::tuple{ make_predicate(std::move(predicates))... };
    return make_predicate([=](const auto& value)
    {
        return check(p, value, std::index_sequence_for<P...>{});
    });
}

template <class T, class Func>
constexpr auto from(Func func)
{
    return [=](const auto& value) -> decltype(auto)
    {
        return func(std::get<T>(value));
    };
}

template <class Pred, class Func>
struct matcher_t
{
    Pred pred;
    Func func;

    constexpr matcher_t(Pred pred, Func func)
        : pred{ std::move(pred) }
        , func{ std::move(func) }
    {
    }

    template <class T>
    constexpr bool check(const T& value) const
    {
        return pred(value);
    }

    template <class T>
    constexpr decltype(auto) get(const T& value) const
    {
        if constexpr (std::is_invocable_v<Func, T>)
        {
            return func(value);
        }
        else if constexpr (std::is_invocable_v<Func>)
        {
            (void)value;
            return func();
        }
        else
        {
            (void)value;
            return func;
        }
    }
};

template <class Pred>
struct where_t
{
    Pred pred;

    constexpr where_t(Pred pred)
        : pred{ std::move(pred) }
    {
    }

    template <class Func>
    constexpr auto operator =(Func func) &&
    {
        return matcher_t{ std::move(pred), std::move(func) };
    }

    constexpr where_t&& operator --(int) &&
    {
        return std::move(*this);
    }

    template <class Func>
    constexpr auto operator >(Func func) &&
    {
        return matcher_t{ std::move(pred), std::move(func) };
    }
};

template <class Pred>
constexpr auto when(Pred pred)
{
    return where_t{ make_predicate(std::move(pred)) };
}

template <class Pred, class Other, class... T>
constexpr auto when(Pred pred, Other other, T... tail)
{
    return when(unpack(std::move(pred), std::move(other), std::move(tail)...));
}

template <class T, class Func>
constexpr auto when_of(Func func)
{
    return when(of<T>()) = from<T>(std::move(func));
}

template <class T, class Pred, class Func>
constexpr auto when_of(Pred pred, Func func)
{
    return when(of<T>(std::move(pred))) = from<T>(std::move(func));
}

template <class T, class P, class F, class... Matchers>
constexpr decltype(auto) match(const T& item, const matcher_t<P, F>& matcher, const Matchers&... matchers)
{
    if (matcher.check(item))
    {
        return matcher.get(item);
    }
    else
    {
        if constexpr (sizeof...(matchers) > 0)
        {
            return match(item, matchers...);
        }
        else
        {
            throw std::runtime_error{ "no match" };
        }
    }
}

long fact(int x)
{
    const auto _ = make_placeholder();

    return match(
        x,
        when(_ > 1) = [=]() { return fact(x - 1) * x; },
        when(_) = 1L);
}

struct Ellipse
{
    float a, b;
};

struct Rect
{
    float a, b;
};

using Shape = std::variant<Ellipse, Rect>;

template <class Cat>
struct cat_to_int
{
};

template <>
struct cat_to_int<std::input_iterator_tag>
{
    static constexpr int value = 1;
};

template <>
struct cat_to_int<std::forward_iterator_tag>
{
    static constexpr int value = 2;
};

template <>
struct cat_to_int<std::bidirectional_iterator_tag>
{
    static constexpr int value = 3;
};

template <>
struct cat_to_int<std::random_access_iterator_tag>
{
    static constexpr int value = 4;
};

template <int V>
struct int_to_cat
{
};

template <>
struct int_to_cat<1>
{
    using type = std::input_iterator_tag;
};

template <>
struct int_to_cat<2>
{
    using type = std::forward_iterator_tag;
};

template <>
struct int_to_cat<3>
{
    using type = std::bidirectional_iterator_tag;
};

template <>
struct int_to_cat<4>
{
    using type = std::random_access_iterator_tag;
};

template <int V>
using int_to_cat_t = typename int_to_cat<V>::type;

template <class Cat0, class Cat1>
struct common_category
{
    using type = int_to_cat_t<std::min(cat_to_int<Cat0>::value, cat_to_int<Cat1>::value)>;
};

template <class Cat0, class Cat1>
using common_category_t = typename common_category<Cat0, Cat1>::type;

void run()
{
    using t = common_category_t<std::forward_iterator_tag, std::input_iterator_tag>;
    std::cout << std::is_same_v<t, std::input_iterator_tag> << std::endl;
}

int main(int argc, char** argv)
{
    const auto start = std::chrono::high_resolution_clock::now();
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    const auto stop = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Elapsed " << elapsed << "ms" << std::endl;
    return 0;
}
