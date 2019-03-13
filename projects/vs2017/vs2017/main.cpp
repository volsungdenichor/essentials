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
#include <complex>
#include <cpp_essentials/geo/intersection.hpp>
#include <cpp_essentials/geo/projection.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/coordinates_conversion.hpp>

#include <cpp_essentials/rs/rs.hpp>

using namespace cpp_essentials;

void run()
{
    static const auto is_prime = [](int n)
    {
        return n >= 2 && !(sq::range(2, n) | sq::count_if([&](auto d) { return n % d == 0; }));
    };

    std::vector<int> vect = { 9, 99, 999 };

    sq::infinite_range()
        | sq::take(10)
        | sq::take_if(is_prime)
        | sq::map([](int x) { return sq::inclusive_range(1, x); })
        | sq::flatten()
        | sq::append(vect)
        | sq::write(std::cout, " ");

    std::cout << std::endl;
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
