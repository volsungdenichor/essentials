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

using namespace cpp_essentials;

geo::square_matrix_2d<double> remap(const geo::rect_2d<double>& source, const geo::rect_2d<double>& dest)
{
    const auto scale = geo::elementwise_divide(dest.size(), source.size());
    const auto dist = dest.location() - source.location();
    return geo::make_scale(scale) * geo::make_translation(dist);
}

std::complex<double> to_complex(const geo::vector_2d<double>& v)
{
    return { v.x(), v.y() };
}

double get_iteration(std::complex<double> z, const std::complex<double> c)
{
    auto i = 0;
    while (i < 255 && std::norm(z) <= 2.0)
    {
        z = z * z + c;
        ++i;
    }
    return i;
}

void run()
{
    gx::rgb_image image{ { 800, 800} };

    const auto transform = remap(image.bounds(), geo::rect_2d<double>{ geo::make_vector(-1.5, -1.5), geo::make_vector(+1.5, +1.5) });
    const auto c = std::complex<double>{ -0.4, 0.6 };

    for (auto iter : core::iterate(image))
    {
        auto[r, b] = 0.3 * iter.location();
        auto g = get_iteration(to_complex(iter.location() * transform), c);
        *iter = gx::rgb_color::proxy{ r, g, b };
    }

    gx::save_bitmap(image, "fractal.bmp");
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
