#ifndef CPP_ESSENTIALS_GEO_VERTEX_CONTAINER_HPP_
#define CPP_ESSENTIALS_GEO_VERTEX_CONTAINER_HPP_

#pragma once

#include <cpp_essentials/geo/detail/vertex_container.traits.hpp>
#include <cpp_essentials/geo/vertex_array.hpp>
#include <cpp_essentials/geo/linear_shape.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

struct segment_count_fn
{
    template <class Shape>
    auto operator ()(const Shape& shape) const -> size_t
    {
        return vertex_container_traits<Shape>::segment_count(shape);
    }
};

struct vertex_count_fn
{
    template <class Shape>
    auto operator ()(const Shape& shape) const -> size_t
    {
        return vertex_container_traits<Shape>::vertex_count(shape);
    }
};

struct get_segment_fn
{
    template <class Shape>
    auto operator ()(const Shape& shape, size_t index) const -> segment_type<Shape>
    {
        return vertex_container_traits<Shape>::get_segment(shape, index);
    }
};

struct get_vertex_fn
{
    template <class Shape>
    auto operator ()(const Shape& shape, size_t index) const -> vertex_type<Shape>
    {
        return vertex_container_traits<Shape>::get_vertex(shape, index);
    }
};

} /* namespace detail */

static constexpr auto segment_count = detail::segment_count_fn{};
static constexpr auto vertex_count = detail::vertex_count_fn{};
static constexpr auto get_segment = detail::get_segment_fn{};
static constexpr auto get_vertex = detail::get_vertex_fn{};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_VERTEX_CONTAINER_HPP_ */