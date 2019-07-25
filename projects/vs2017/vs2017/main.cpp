#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

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

using namespace cpp_essentials;

void run()
{
    geo::dcel<float> dcel;
    auto v0 = dcel.add_vertex({ 5, 5 });
    auto v1 = dcel.add_vertex({ 10, 0 });
    auto v2 = dcel.add_vertex({ 0, 0 });
    auto v3 = dcel.add_vertex({ 5, 2 });

    auto f = dcel.add_face({ v0, v1, v2 });
    dcel.add_boundary({ v0, v2, v1 });

    dcel.divide_face(f, v3);

#if 1
    for (auto&& vertex : dcel.vertices())
    {
        std::cout << vertex << std::endl;

        std::cout << "out_halfedges" << std::endl;
        for (auto&& he : vertex.out_halfedges())
        {
            std::cout << "  HE" << he.id << std::endl;
        }

        std::cout << "in_halfedges" << std::endl;
        for (auto&& he : vertex.in_halfedges())
        {
            std::cout << "  HE" << he.id << std::endl;
        }

        std::cout << "incident_faces" << std::endl;
        for (auto&& f : vertex.incident_faces())
        {
            std::cout << "  F" << f.id << std::endl;
        }
    }
#endif

#if 1
    for (auto&& f : dcel.faces())
    {
        std::cout << f << " " << f.as_polygon() << std::endl;
        std::cout << "  incident_faces" << std::endl;
        std::cout << "    " << core::delimit(f.incident_faces() | sq::map([](auto&& x) { return x.id; }), ", ") << std::endl;
    }
#endif

#if 0
    for (auto&& h : dcel.halfedges())
    {
        std::cout << "H " << h.id << " " << h.as_segment() << std::endl;
        std::cout << "H " << h.incident_face() << std::endl;
    }
#endif
}

void run2()
{
    const auto r = core::make_generator([state{ std::pair{ 1, 1 } }]() mutable->core::optional<int>
    {
        const auto[prev, current] = state;
        state = std::pair{ current, prev + current };
        return current;
    });

    std::vector<int> fib = r | sq::take(10);
    std::cout << core::delimit(fib, " ") << std::endl;

    core::copy_n(r, 10, core::output(std::cout, " "));
}

int main()
{
    try
    {
        run2();
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    return 0;
}
