#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/cc/cc.hpp>

#include <cpp_essentials/math/arithmetic_functors.hpp>
#include <cpp_essentials/math/functors.hpp>

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/output.hpp>
#include <cpp_essentials/core/tuple.hpp>

#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/format.hpp>

using namespace cpp_essentials;

void run()
{
    core::print("{0:width=20|fill=_} = 0x{1:hex|width=4|fill=0}\n", "value", 32);
    core::print("{0:width=20|fill=_} = 0x{1:hex|width=4|fill=0}\n", "other_value", 64);
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