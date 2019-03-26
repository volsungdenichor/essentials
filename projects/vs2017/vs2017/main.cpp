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
#include <complex>
#include <cpp_essentials/geo/intersection.hpp>
#include <cpp_essentials/geo/projection.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/coordinates_conversion.hpp>
#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/core/expected.hpp>

using namespace cpp_essentials;

core::expected<float, std::string> _square_root(float x)
{
    if (x < 0)
    {
        return core::make_unexpected(core::str("non-negative required, got ", x));
    }
    return std::sqrt(x);
}

core::expected<float&, std::string> square_root(float x)
{
    static float a = 42;
    static float b = 66;
    if (x < -5)
    {
        return b;
    }
    if (x > 3)
    {
        return a;
    }
    return core::make_unexpected("?");
}

void run()
{
    std::vector<std::string_view> vect = { "1", "2", "x", "4", "xx", "5" };

    auto res = vect
        | sq::flat_map(core::try_parse<int>)
        //| sq::join(", ");
        | sq::map(core::plus('A'))
        | sq::map(core::cast<char>)
        | sq::join(", ");

    core::println("{{ {} }}", res);


}

int main()
{
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::exception_ptr o;
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    return 0;
}
