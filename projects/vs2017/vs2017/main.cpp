#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>
#include <atomic>

#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/core/string_views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/core/string_functors.hpp>
#include <string_view>
#include <cpp_essentials/meta/serialization_json.hpp>
#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/morphological_operations.hpp>
#include <cpp_essentials/gx/kernels.hpp>
#include <cpp_essentials/gx/lookup_table.hpp>
#include <cpp_essentials/gx/drawing_context.hpp>
#include <cpp_essentials/gx/filters.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <complex>
#include <cpp_essentials/geo/intersection.hpp>
#include <cpp_essentials/geo/projection.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/coordinates_conversion.hpp>
#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/core/expected.hpp>
#include <cpp_essentials/graphs/algorithm.hpp>
#include <cpp_essentials/graphs/tree.hpp>
#include <cpp_essentials/gx/filters.hpp>
#include <cpp_essentials/core/optional_helpers.hpp>
#include <cpp_essentials/core/string_functors.hpp>
#include <cpp_essentials/core/any_range.hpp>

#include <cpp_essentials/kine/animation.hpp>
#include <cpp_essentials/geo/orientation.hpp>
#include <cpp_essentials/geo/clamp.hpp>
#include <cpp_essentials/core/generator.hpp>
#include <cpp_essentials/geo/dcel.hpp>
#include <cpp_essentials/core/program_args.hpp>
#include <cpp_essentials/core/match.hpp>

#include <cpp_essentials/arrays/array.hpp>
#include <cpp_essentials/arrays/transformations.hpp>
#include <cpp_essentials/geo/triangulation.hpp>
#include <cpp_essentials/core/debug_utils.hpp>
#include <cpp_essentials/core/channel.hpp>
#include <cpp_essentials/core/task_queue.hpp>
#include <cpp_essentials/core/event_aggregator.hpp>

#include <cpp_essentials/carto/coords_array.hpp>
#include <cpp_essentials/math/binomial.hpp>

#include <cpp_essentials/fmt/format.hpp>

#include <cpp_essentials/math/regression.hpp>
#include <cpp_essentials/chrono/calendars.hpp>

using namespace cpp_essentials;

struct Person
{
    std::string name;
};

std::ostream& operator <<(std::ostream& os, const Person& item)
{
    return os << item.name;
}

template <class T>
int address(T& item)
{
    return (unsigned long long)(std::addressof(item)) % 1000;
}

void run()
{
    auto persons = std::vector<Person>{
        { "Adam" },
        { "Ola" },
        { "Stefan" },
        { "Regina" },
        { "Anna" },
        { "Oskar" },
    };

    for (const auto& p : persons)
    {
        std::cout << p << " 0x" << address(p) << std::endl;
    }

    static const auto conv = [](const Person& p) -> const Person&
    {
        return p;
    };

    const auto g = persons | sq::group_by([](const Person& p) { return p.name[0]; });

    for (const auto&[k, v] : g)
    {
        std::cout << k << " : " << std::endl;
        for (const auto& val : v)
        {
            std::cout << "  " << val << " 0x" << address(val) << std::endl;
        }
    }
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
