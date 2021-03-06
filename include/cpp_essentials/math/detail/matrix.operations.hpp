#ifndef CPP_ESSENTIALS_MATH_DETAIL_MATRIX_OPERATIONS_HPP_
#define CPP_ESSENTIALS_MATH_DETAIL_MATRIX_OPERATIONS_HPP_

#pragma once

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/optional.hpp>

#include <cpp_essentials/math/detail/matrix.base.hpp>
#include <cpp_essentials/math/detail/matrix.access.hpp>

#ifdef __GNUC__
#  undef minor
#endif

namespace cpp_essentials::math
{

namespace detail
{

struct minor_fn
{
    template <class T, size_t R, size_t C>
    auto operator ()(const matrix<T, R, C>& item, size_t row, size_t col) const -> matrix<T, R - 1, C - 1>
    {
        static_assert(R > 1, "minor: invalid row.");
        static_assert(C > 1, "minor: invalid col.");

        EXPECTS(row < R);
        EXPECTS(col < C);

        matrix<T, R - 1, C - 1> result;

        for (size_t r = 0; r < result.row_count(); ++r)
        {
            for (size_t c = 0; c < result.col_count(); ++c)
            {
                result(r, c) = item(r + (r < row ? 0 : 1), c + (c < col ? 0 : 1));
            }
        }

        return result;
    }
};

struct determinant_fn
{
    template <class T>
    auto operator ()(const square_matrix<T, 1>& item) const
    {
        return get<0, 0>(item);
    }

    template <class T>
    auto operator ()(const square_matrix<T, 2>& item) const
    {
        return
            + get<0, 0>(item) * get<1, 1>(item)
            - get<0, 1>(item) * get<1, 0>(item);
    }

    template <class T>
    auto operator ()(const square_matrix<T, 3>& item) const
    {
        return
            + get<0, 0>(item) * get<1, 1>(item) * get<2, 2>(item)
            + get<0, 1>(item) * get<1, 2>(item) * get<2, 0>(item)
            + get<0, 2>(item) * get<1, 0>(item) * get<2, 1>(item)
            - get<0, 2>(item) * get<1, 1>(item) * get<2, 0>(item)
            - get<0, 0>(item) * get<1, 2>(item) * get<2, 1>(item)
            - get<0, 1>(item) * get<1, 0>(item) * get<2, 2>(item);
    }

    template <class T, size_t D>
    auto operator ()(const square_matrix<T, D>& item) const
    {
        static constexpr auto _minor = minor_fn{};

        auto sum = T{};

        for (size_t i = 0; i < D; ++i)
        {
            sum += (i % 2 == 0 ? 1 : -1) * item(0, i) * (*this)(_minor(item, 0, i));
        }

        return sum;
    }
};

struct is_invertible_fn
{
    template <class T, size_t D>
    bool operator ()(const square_matrix<T, D>& value) const
    {
        static constexpr auto _determinant = determinant_fn{};
        return _determinant(value);
    }
};

struct invert_fn
{
    template <class T, size_t D>
    auto operator ()(const square_matrix<T, D>& value) const -> core::optional<square_matrix<T, D>>
    {
        static constexpr auto _determinant = determinant_fn{};
        static constexpr auto _minor = minor_fn{};

        auto det = _determinant(value);

        if (!det)
        {
            return{};
        }

        square_matrix<T, D> result;

        for (size_t r = 0; r < D; ++r)
        {
            for (size_t c = 0; c < D; ++c)
            {
                result(c, r) = T((r + c) % 2 == 0 ? 1 : -1) * _determinant(_minor(value, r, c)) / det;
            }
        }

        return result;
    }
};

struct transpose_fn
{
    template <class T, size_t R, size_t C>
    auto operator ()(const matrix<T, R, C>& item) const -> matrix<T, C, R>
    {
        matrix<T, C, R> result;

        for (size_t r = 0; r < item.row_count(); ++r)
        {
            for (size_t c = 0; c < item.col_count(); ++c)
            {
                result(c, r) = item(r, c);
            }
        }

        return result;
    }
};

struct transform_fn
{
    template <class T, size_t R, size_t C, class Func>
    auto operator ()(const matrix<T, R, C>& item, Func&& func) const -> matrix<std::invoke_result_t<Func, T>, R, C>
    {
        matrix<std::invoke_result_t<Func, T>, R, C> result;
        core::transform(item._data, result._data.begin(), func);
        return result;
    }
};

} /* namespace detail */

static constexpr auto minor = detail::minor_fn{};
static constexpr auto determinant = detail::determinant_fn{};
static constexpr auto is_invertible = detail::is_invertible_fn{};
static constexpr auto invert = detail::invert_fn{};
static constexpr auto transpose = detail::transpose_fn{};
static constexpr auto transform = detail::transform_fn{};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_MATRIX_OPERATIONS_HPP_ */