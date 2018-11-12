#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

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


void run()
{
    gx::rgb_image img = gx::load_bitmap(R"(D:\Users\Krzysiek\Pictures\raccoons.bmp)");
    auto dc = gx::make_drawing_context(img);
    dc.draw("Ala", { 8, 8 }, gx::colors::orange_red);
    dc.draw(geo::segment_2d<int>{ { 10, 10 }, { 40, 50 } }, gx::colors::cornflower_blue);

    
    std::cout << img.size() << std::endl;
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
