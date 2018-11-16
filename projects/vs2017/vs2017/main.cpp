#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/meta/meta.hpp>

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

using Json = nlohmann::json;

struct Test
{
    std::string id;
    int a;
    double b;
    std::optional<std::string> text;
};

template <>
inline auto meta::register_members<Test>()
{
    using namespace meta;
    return members(
        member(&Test::a, "a"),
        member(&Test::b, "b"),
        member(&Test::text, "text"));
}

enum class Enum
{
    A,
    B,
};

template <>
inline auto meta::register_enum_values<Enum>()
{
    using namespace meta;
    return enum_values(
        enum_value(Enum::A, "A"),
        enum_value(Enum::B, "B"));
}

void run()
{
    std::cout << meta::name_of(&Test::text) << std::endl;
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
