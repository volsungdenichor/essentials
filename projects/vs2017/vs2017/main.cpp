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

using namespace cpp_essentials;

void print(core::span<char> text)
{
    auto x = core::find_if(core::return_found_next, text, core::equal_to('Z'));
    std::cout << x.size() << std::endl;
}

int main()
{
    std::string text = "Ala ma psychicznie chora xciotke";
    print({ text.data() + 2, text.size() - 2});
    return 0;
}

