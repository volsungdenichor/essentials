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
    std::vector<geo::dcel<float>::vertex_id> v(11);
    for (int i = 0; i <= 10; ++i)
        v.at(i) = dcel.add_vertex({});

    const auto add_face = [&](int a, int b, int c)
    {
        dcel.add_face({ v[a], v[b], v[c] });
    };

    const auto add_boundary = [&](const std::vector<int>& indices)
    {
        auto vertices = indices | sq::map([&](int i) { return v[i]; }) | sq::to_vector();
        dcel.add_boundary(vertices);
    };

    add_face(0, 1, 4);
    add_face(1, 2, 5);
    add_face(2, 3, 6);
    
    add_face(1, 5, 4);
    add_face(2, 6, 5);

    add_face(4, 5, 8);
    add_face(5, 9, 8);
    add_face(5, 6, 9);

    add_face(4, 8, 7);
    add_face(6, 10, 9);

    add_boundary({ 0, 4, 7, 8, 9, 10, 6, 3, 2, 1 });
    

#if 1
    for (auto&& vertex : dcel.vertices())
    {
        std::cout << vertex << std::endl;

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
        std::cout << "    " << core::delimit(f.adjacent_faces() | sq::map([](auto&& x) { return x.id; }), ", ") << std::endl;
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

int main()
{
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    return 0;
}
