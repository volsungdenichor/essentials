#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/meta/serialization.hpp>

#include <cpp_essentials/math/functors.hpp>
#include <cpp_essentials/core/output.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/chunk.hpp>

#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/elementwise.hpp>

#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/math/polynomial.hpp>

#include <json.hpp>
#include <cpp_essentials/math/regression.hpp>
#include <cpp_essentials/arrays/array.hpp>
#include <cpp_essentials/arrays/transformations.hpp>
#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <cpp_essentials/gx/drawing_context.hpp>

using namespace cpp_essentials;

enum class Color
{
    red,
    green,
    blue,
};

struct Test
{
    int a;
    std::string b;
    std::vector<Color> colors;
};

template <>
inline auto meta::register_type<Test>()
{    
    return structure(
        "Test",
        member(&Test::a, "a"),
        member(&Test::b, "b"),
        member_or_default(&Test::colors, "colors")
    );
}

template <>
inline auto meta::register_type<Color>()
{    
    return enumeration(
        "Color",
        enum_value(Color::red, "red"),
        enum_value(Color::green, "green"),
        enum_value(Color::blue, "blue")
    );
}




void run()
{
    Test other = R"({ "a": 0, "b": "", "colors": ["red", "red", "green", "red"] })"_json;

    std::cout << "--" << std::endl;
    std::cout << nlohmann::json{ other }.dump(2) << std::endl;
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
