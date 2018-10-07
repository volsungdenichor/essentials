#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/core/output.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/chunk.hpp>

#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/elementwise.hpp>

using namespace cpp_essentials;

static constexpr core::adaptable<core::detail::pairwise_t> pairwise = {};

void run()
{
    sq::range(10)
        | sq::index(10)
        | sq::map([](auto&& item) { return std::vector<int> { item.index, item.value }; })
        | sq::to_vector()
        | sq::flatten()
        | sq::chunk(4, 3)
        | sq::for_each([](auto&& r) { std::cout << core::delimit(r, " ") << std::endl; });
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
