#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <chrono>
#include <type_traits>
#include <random>

#include <cpp_essentials/fmt/format.hpp>

#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <cpp_essentials/gx/histogram_operations.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/math/polynomial.hpp>

#include <cpp_essentials/geo/bezier.hpp>
#include <cpp_essentials/geo/algorithm.hpp>

#include <cpp_essentials/term/term.hpp>

#include <cpp_essentials/proc/perlin.hpp>
#include <cpp_essentials/proc/distribution.hpp>

#include <cpp_essentials/geo/triangulation.hpp>
#include <cpp_essentials/geo/voronoi.hpp>

#include <cpp_essentials/gx/drawing_context.hpp>

using namespace cpp_essentials;

template <class T, class Func>
gx::image<T> generate_image(const gx::image_size_t& size, Func func)
{
    gx::image<T> result{ size };
    for (auto it : core::views::iterate(result))
    {
        *it = func(it.location());
    }
    return result;
}

void run()
{
    static const auto rnd = proc::uniform(-50.F, 450.F);
    
    static const auto create_vertex = [&]() -> geo::vector_2d<float>
    {
        return { rnd(), rnd() };
    };
    
    const auto points = core::make_infinite_generator(create_vertex) | sq::take(500) | sq::to_vector();

    const auto voronoi = geo::voronoi(points);

    gx::rgb_image image{ { 400, 400 } };
    auto ctx = gx::make_drawing_context( image.view() );

    for (const auto f : voronoi.faces())
    {
        ctx.draw(f.as_polygon(), gx::colors::white);
    }

    char ch;
    (void)ch;
}

int main(int argc, char** argv)
{
    const auto start = std::chrono::high_resolution_clock::now();
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    const auto stop = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Elapsed " << elapsed << "ms" << std::endl;
    return 0;
}
