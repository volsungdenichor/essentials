#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <chrono>
#include <type_traits>
#include <random>

#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <cpp_essentials/gx/histogram_operations.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/math/polynomial.hpp>

using namespace cpp_essentials;

struct fade
{
    template <class T>
    T operator ()(T t) const
    {
        static const math::polynomial<T, 5> poly{ 0, 0, 0, +10, -15, +6 };
        return poly(t);
    }
};

template <class T>
T grad(int hash, T x, T y, T z)
{
    int h = hash & 0xF;

    T u = h < 0x08 ? x : y;
    T v = h < 4 ? y : h == 0x0c || h == 0x0e ? x : z;

    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

template <class T>
T grad(int hash, const geo::vector<T, 3>& vect)
{
    return grad(hash, vect.x(), vect.y(), vect.z());
}

class permutation
{
public:
    permutation(std::vector<int> values)
        : _values{ std::move(values) }
    {
        EXPECTS(_values.size() == 256);
    }

    permutation(const permutation&) = default;
    permutation(permutation&&) = default;

    int operator [](size_t index) const
    {
        return _values[index % 256];
    }

    static permutation create(int seed)
    {
        std::default_random_engine engine(seed);

        std::vector<int> result(256);
        core::iota(result, 0);
        core::shuffle(result, engine);
        return result;
    }

private:
    std::vector<int> _values;
};

struct perlin_noise
{
    permutation _permutation;

    perlin_noise(permutation permutation)
        : _permutation{ std::move(permutation) }
    {
    }

    template <class T>
    T operator ()(const geo::vector_3d<T>& location) const
    {
        using core::lerp;

        static const std::array<geo::vector<int, 3>, 8> translations =
        {
            geo::vector_3d<int>{ 0, 0, 0 },
            geo::vector_3d<int>{ 1, 0, 0 },
            geo::vector_3d<int>{ 0, 1, 0 },
            geo::vector_3d<int>{ 1, 1, 0 },
            geo::vector_3d<int>{ 0, 0, 1 },
            geo::vector_3d<int>{ 1, 0, 1 },
            geo::vector_3d<int>{ 0, 1, 1 },
            geo::vector_3d<int>{ 1, 1, 1 },
        };

        const auto pos = geo::vector_3d<int>{ location._data | sq::map(math::floor) };
        const auto rel_pos = geo::vector_3d<T>{ location._data | sq::map(math::fractional_part) };
        const auto p = geo::vector_3d<T>{ rel_pos._data | sq::map(fade{}) };

        const auto vert = std::invoke([&]()
        {
            std::array<int, 2> v;
            sq::range(2) | sq::map([&](int i) { return _permutation[pos.x() + i] + pos.y(); }) | sq::overwrite(v);

            std::array<int, 4> vert;
            sq::range(4) | sq::map([&](int i) { return _permutation[v[i % 2] + (i / 2)] + pos.z(); }) | sq::overwrite(vert);
            return vert;
        });

        return
            lerp(p[2],
                lerp(p[1],
                    lerp(p[0],
                        grad(_permutation[vert[0] + 0], rel_pos - translations[0]),
                        grad(_permutation[vert[1] + 0], rel_pos - translations[1])),
                    lerp(p[0],
                        grad(_permutation[vert[2] + 0], rel_pos - translations[2]),
                        grad(_permutation[vert[3] + 0], rel_pos - translations[3]))),
                lerp(p[1],
                    lerp(p[0],
                        grad(_permutation[vert[0] + 1], rel_pos - translations[4]),
                        grad(_permutation[vert[1] + 1], rel_pos - translations[5])),
                    lerp(p[0],
                        grad(_permutation[vert[2] + 1], rel_pos - translations[6]),
                        grad(_permutation[vert[3] + 1], rel_pos - translations[7]))));
    }

    template <class T>
    T operator  ()(const geo::vector_2d<T>& location) const
    {
        return (*this)(convert(location));
    }

    template <class T>
    static geo::vector_3d<T> convert(const geo::vector_2d<T>& v)
    {
        return { v.x(), v.y(), T{} };
    }
};

template <class T>
struct perlin_noise_ext
{
    perlin_noise inner;
    int octaves;
    T persistence;
    T frequency;

    perlin_noise_ext(perlin_noise inner, int octaves, T persistence, T frequency)
        : inner{ std::move(inner) }
        , octaves{ octaves }
        , persistence{ persistence }
        , frequency{ frequency }
    {
    }

    template <class T>
    T operator()(const geo::vector_3d<T>& location) const
    {
        T sum = math::zero;
        T amplitude = math::one;
        T f = frequency;

        for (int i = 0; i < octaves; ++i)
        {
            sum += inner(location * f) * amplitude;
            amplitude *= persistence;
            f *= 2;
        }

        T max_value = amplitude * octaves;

        return sum / max_value;
    }


    template <class T>
    T operator()(const geo::vector_2d<T>& location) const
    {
        return (*this)(perlin_noise::convert(location));
    }
};

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

void _run()
{
    const auto noise = perlin_noise_ext<float>{ perlin_noise{ permutation::create(888) }, 4, 0.5F, 0.5F };


    const auto image = generate_image<gx::rgb_color>({ 500, 500 }, [&](const auto& loc)
    {
        const auto v = noise(loc / 200.F) * 0.6F;
        return gx::colors::red * v;
    });

    gx::save_bitmap(image, "o.bmp");
}

void run()
{
    gx::byte_image img = gx::load_bitmap(R"(D:\Users\Krzysiek\Pictures\hippie_2.bmp)");

    gx::save_bitmap(img, R"(D:\Users\Krzysiek\Pictures\hippie_gray.bmp)");
    gx::otsu(img);
    gx::save_bitmap(img, R"(D:\Users\Krzysiek\Pictures\hippie_eq.bmp)");
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
