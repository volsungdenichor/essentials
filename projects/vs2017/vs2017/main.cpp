#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/arithmetic_functors.hpp>
#include <cpp_essentials/core/math_functors.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/serialization.hpp>

using namespace cpp_essentials;

int main()
{
    core::transform(std::vector<int>{2, 3, 4}, std::ostream_iterator<std::string>{std::cout, " "}, core::to_string);
    return 0;
}

