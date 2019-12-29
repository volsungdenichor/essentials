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
#include <cpp_essentials/chrono/calendars.hpp>

template <class T>
using opt = cpp_essentials::core::optional<T>;

template <class T>
struct range_t
{
public:
    using value_type = T;

    using generator_type = std::function<opt<value_type>()>;

    range_t(generator_type generator)
        : _generator{ std::move(generator) }
    {
    }

    opt<value_type> next()
    {
        return _generator();
    }

    template <class Func>
    auto map(Func func) -> range_t<std::invoke_result_t<Func, value_type>>
    {
        using type = std::invoke_result_t<Func, value_type>;
        return{ [=, *this]() mutable->opt<type>
        {
            if (auto v = next())
                return func(*v);
            else
                return core::none;
        } };
    }

    template <class Pred>
    auto filter(Pred pred) -> range_t
    {
        return { [=, *this]() mutable->opt<value_type>
        {
            while (auto v = next())
            {
                if (pred(*v))
                    return v;
            }
            return core::none;
        } };
    }

    template <class Func>
    auto filter_map(Func func)
    {
        using type = cpp_essentials::core::underlying_type_t<std::invoke_result_t<Func, value_type>>;
        return range_t<type>{ [=, *this]() mutable->opt<type>
        {
            while (true)
            {
                auto v = next();

                if (!v)
                    break;

                if (auto result = func(*v); result)
                    return result;
            }

            return core::none;
        } };
    }

    template <class U = T, class = typename U::value_type>
    auto flatten() -> range_t<typename U::value_type>
    {
        using type = typename U::value_type;
        opt<value_type> inner = {};

        return { [=, *this]() mutable->opt<type>
        {
             while (true)
             {
                if (!inner)
                {
                    if (auto outer = next(); outer)
                    {
                        inner = *outer;
                        continue;
                    }
                    else
                    {
                        return core::none;
                    }
                }

                if (auto result = inner->next(); result)
                {
                    return result;
                }
                else
                {
                    inner = core::none;
                    continue;
                }
             }
        } };
    }

    template <class Func>
    auto flat_map(Func func)
    {
        return map(func).flatten();
    }

    auto take(int count) -> range_t
    {
        return { [=, *this]() mutable->opt<value_type>
        {
            while (count > 0)
            {
                --count;
                return next();
            }
            return core::none;
        } };
    }

    auto drop(int count) -> range_t
    {
        return { [=, *this]() mutable->opt<value_type>
        {
            while (count > 0)
            {
                --count;
                next();
            }
            return next();
        } };
    }

    template <class Pred>
    auto take_while(Pred pred) -> range_t
    {
        return { [=, *this]() mutable->opt<value_type>
        {
            if (auto v = next())
            {
                if (pred(*v))
                    return v;
                else
                    return core::none;
            }
            return core::none;
        } };
    }

    template <class Pred>
    auto drop_while(Pred pred) -> range_t
    {
        return { [=, *this]() mutable->opt<value_type>
        {
            while (auto v = next())
            {
                if (pred(*v))
                    return v;
            }
            return next();
        } };
    }

    template <class U>
    auto zip_with(range_t<U> other) -> range_t<std::pair<value_type, U>>
    {
        using type = std::pair<value_type, U>;
        return { [=, *this]() mutable->opt<type>
        {
            auto n = next();
            auto o = other.next();
            if (n && o)
                return type{ *n, *o };
            else
                return core::none;
        } };
    }

    template <class Func>
    void for_each(Func func)
    {
        while (auto v = next())
        {
            func(*v);
        }
    }

    template <class Pred>
    bool all(Pred pred)
    {
        while (auto v = next())
        {
            if (!pred(*v))
                return false;
        }
        return true;
    }

    template <class Pred>
    bool any(Pred pred)
    {
        while (auto v = next())
        {
            if (pred(*v))
                return true;
        }
        return false;
    }

    template <class Pred>
    bool none(Pred pred)
    {
        while (auto v = next())
        {
            if (pred(*v))
                return false;
        }
        return true;
    }

    template <class U = value_type, class Accumulator = std::plus<>>
    auto accumulate(U init = {}, Accumulator accumulator = {})
    {
        while (auto v = next())
        {
            init = accumulator(init, *v);
        }
        return init;
    }

    template <class Output>
    void copy(Output out)
    {
        while (auto v = next())
        {
            *out++ = *v;
        }
    }

private:
    generator_type _generator;
};

using namespace cpp_essentials;

template <class T = std::uint64_t>
auto fibonacci() -> range_t<T>
{
    auto state = std::pair<T, T>{ 1, 1 };

    return { [=]() mutable -> core::optional<T>
    {
        auto[cur, nxt] = state;
        state = { nxt, cur + nxt };
        return cur;
    } };
}

template <class Iter>
auto from(Iter b, Iter e) -> range_t<cc::iter_ref<Iter>>
{
    using type = cc::iter_ref<Iter>;
    return { [=]() mutable -> core::optional<type>
    {
        if (b == e)
            return core::none;
        else
            return *b++;
    } };
}

template <class Range>
auto from(Range&& range)
{
    return from(std::begin(range), std::end(range));
}

template <class T>
auto range(T from, T to) -> range_t<T>
{
    return { [=]() mutable -> core::optional<T>
    {
        if (from >= to)
            return core::none;
        else
            return from++;
    } };
}

template <class T = int>
auto iota(T from = {}) -> range_t<T>
{
    return { [=]() mutable -> core::optional<T>
    {
        return from++;
    } };
}

template <class T>
auto range(T to) -> range_t<T>
{
    return range(T{}, to);
}

using Triple = std::tuple<int, int, int>;

auto pythagorean_triples() -> range_t<Triple>
{
    return iota(1)
        .flat_map([](int z)
    {
        return range(1, z + 1)
            .flat_map([=](int x)
        {
            return range(x, z + 1)
                .filter_map([=](int y) -> core::optional<Triple>
            {
                if (x * x + y * y == z * z)
                    return Triple{ x, y, z };
                else
                    return core::none;
            });
        });
    })
        ;
}

void run()
{
    pythagorean_triples()
        .take_while([](const auto& triple) { return std::get<2>(triple) < 100; })
        .copy(core::output(std::cout, "\n"));

    core::ints(1)
        | sq::map([](int z)
    {
        return sq::range(1, z + 1)
            | sq::map([=](int x)
        {
            return sq::range(x, z + 1)
                | sq::map([=](int y) -> Triple
            {
                return Triple{ x, y, z };
            });
        })
            | sq::flatten();
    })
        | sq::flatten();
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
