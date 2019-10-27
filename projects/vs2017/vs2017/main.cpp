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

using namespace cpp_essentials;

using Json = nlohmann::json;

struct Repo
{
public:
    static const inline std::string id_v = "$id";
    static const inline std::string parent_v = "$parent";

    void add(const Json& json)
    {
        _map[json.at(id_v)] = json;
    }

    Json operator [](const std::string& id) const
    {
        return resolve(id);
    }

private:
    Json resolve(const std::string& id) const
    {
        const auto replace_parent = [this](const std::string& parent_id, Json& result)
        {
            const auto parent_json = resolve(parent_id);
            for (const auto& it : parent_json.items())
            {
                if (!result.count(it.key()))
                {
                    result[it.key()] = it.value();
                }
            }
        };

        Json result = _map.at(id);
        if (auto it = result.find(parent_v); it != result.end())
        {
            const auto& parent_value = it.value();
            if (parent_value.is_string())
            {
                replace_parent(parent_value, result);
            }
            else if (parent_value.is_array())
            {
                for (const auto& p : parent_value)
                {
                    replace_parent(p, result);
                }
            }
        }
        result.erase(parent_v);
        result.erase(id_v);
        return result;
    }

    std::map<std::string, Json> _map;
};

struct Book
{
    std::string title;
    int year;
};

void run()
{
    for (int i = 0; i <= 100; ++i)
        std::cout << math::binomial(17, i) << std::endl;
}

int main(int argc, char** argv)
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
