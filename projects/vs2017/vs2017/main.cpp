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
#include <cpp_essentials/gx/morphological_operations.hpp>
#include <cpp_essentials/gx/kernels.hpp>
#include <cpp_essentials/gx/lookup_table.hpp>
#include <cpp_essentials/gx/drawing_context.hpp>
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

using namespace cpp_essentials;

struct BoneData
{
    std::string name;
    float length = 1.F;
    float rotation = 0.F;
    float position_x = 0.F;
    float position_y = 0.F;
    float scale_x = 1.F;
    float scale_y = 1.F;
};

struct Bone
{
    using Id = std::size_t;
    static const Id None = static_cast<Id>(-1);
    
    Id id;
    Id parent;
    BoneData data;

    operator Id() const
    {
        return id;
    }
};

std::ostream& operator <<(std::ostream& os, const BoneData& item)
{
    return os << item.name;
}

std::ostream& operator <<(std::ostream& os, const Bone& item)
{
    return os << item.data;
}

class Skeleton
{
public:
    Skeleton() = default;

    using const_iterator = std::vector<Bone>::const_iterator;

    const_iterator begin() const
    {
        return _bones.begin();
    }

    const_iterator end() const
    {
        return _bones.end();
    }

    size_t size() const
    {
        return _bones.size();
    }

    const Bone& operator [](Bone::Id id) const
    {
        return _bones.at(id);
    }

    const Bone& at(Bone::Id id) const
    {
        return _bones.at(id);
    }

    Bone::Id find(const std::string& name) const
    {
        return _bones
            | sq::drop_until([&](const Bone& b) { return b.data.name == name; })
            | sq::map(&Bone::id)
            | sq::front_or(Bone::None);
    }

    Bone::Id append(Bone::Id parent, BoneData data)
    {
        return append(std::move(data), parent);
    }

    auto children(Bone::Id id) const
    {
        return _bones | sq::take_if([=](const Bone& bone) { return bone.parent == id; });
    }

private:
    Bone::Id append(BoneData data, Bone::Id parent)
    {
        if (parent == Bone::None)
        {
            EXPECTS(_bones.empty());
        }
        Bone::Id id = _bones.size();
        Bone bone{ id, parent, std::move(data) };
        _bones.push_back(std::move(bone));
        return id;
    }

    std::vector<Bone> _bones;
};

void from_json(const nlohmann::json& json, BoneData& item)
{
    item.name = json.at("name").get<std::string>();
    item.length = json.value("length", 1.F);
    item.position_x = json.value("x", 0.F);
    item.position_y = json.value("x", 0.F);
    item.rotation = json.value("rot", 0.F);
    item.scale_x = json.value("scale_x", 1.F);
    item.scale_y = json.value("scale_y", 1.F);
}

Skeleton parse_skeleton(const nlohmann::json& json)
{
    Skeleton result;
    for (const auto& item : json)
    {
        auto data = item.get<BoneData>();
        auto parent = item.find("parent");
        result.append(parent != item.end() ? result.find(*parent) : Bone::None, std::move(data));
    }
    return result;
}

using Matrix = geo::square_matrix_2d<float>;

struct BoneState
{
    Matrix local;
    Matrix global;
};

using Animation = core::function<float, float>;

struct BoneAnimation
{
    Animation rotation;
    Animation position_x;
    Animation position_y;
    Animation scale_x;
    Animation scale_y;

    geo::square_matrix_2d<float> operator ()(float t) const
    {
        float r = rotation(t);
        geo::vector_2d<float> position = { position_x(t), position_y(t) };
        geo::vector_2d<float> scale = { scale_x(t), scale_y(t) };

        return geo::make_rotation(r) * geo::make_translation(position) * geo::make_scale(scale);
    }
};

using SkeletonAnimation = std::vector<BoneAnimation>;

std::vector<Matrix> get_local_state(const Skeleton& skeleton, const SkeletonAnimation& animation, float t)
{
    EXPECTS(core::size(skeleton) == core::size(animation));
    (void)skeleton;
    return animation | sq::map([&](const auto& a) { return a(t); });   
}

std::vector<Matrix> get_global_state(const Skeleton& skeleton, const std::vector<Matrix>& local_states)
{
    EXPECTS(core::size(skeleton) == core::size(local_states));

    std::vector<Matrix> global_states(local_states.size());
    
    static constexpr float uninitialized = std::numeric_limits<float>::min();
    
    for (auto& item : global_states)
    {
        item[0] = uninitialized;
    }

    const auto is_initialized = [](const Matrix& matrix)
    {
        return matrix[0] != uninitialized;
    };

    const core::function<Matrix, Bone> get_local = [&](const Bone& bone) -> Matrix
    {
        return local_states.at(bone.id);
    };

    const core::function<Matrix, Bone> get_global = [&](const Bone& bone) -> Matrix
    {
        Matrix& result = global_states.at(bone.id);

        if (!is_initialized(result))
        {
            result = bone.parent != Bone::None
                ? get_local(bone) * get_global(skeleton.at(bone.parent))
                : get_local(bone);
        }

        return result;
    };

    for (const auto&[bone, local] : core::zip(skeleton, local_states, core::tie))
    {
        global_states.push_back(get_global(bone));
    }

    return global_states;
}

std::vector<Matrix> get_global_state(const Skeleton& skeleton, const SkeletonAnimation& animation, float t)
{
    EXPECTS(core::size(skeleton) == core::size(animation));

    return get_global_state(skeleton, get_local_state(skeleton, animation, t));
}

std::vector<BoneState> get_state(const Skeleton& skeleton, const SkeletonAnimation& animation, float t)
{
    EXPECTS(core::size(skeleton) == core::size(animation));

    auto local = get_local_state(skeleton, animation, t);
    auto global = get_global_state(skeleton, local);
    return core::zip(local, global, [](auto loc, auto glob) -> BoneState { return { loc, glob }; });
}

void run()
{
    auto skeleton = parse_skeleton(
        R"(
        [
            { "name": "pelvis" },
            { "name": "trunk", "parent": "pelvis" },
            { "name": "r_arm", "parent": "trunk" },
            { "name": "r_forearm", "parent": "r_arm" },
            { "name": "r_hand", "parent": "r_forearm" },
            { "name": "l_arm", "parent": "trunk" },
            { "name": "l_forearm", "parent": "l_arm" },
            { "name": "l_hand", "parent": "l_forearm" },
            { "name": "r_thigh", "parent": "pelvis" },
            { "name": "r_shin", "parent": "r_thigh" },
            { "name": "r_foot", "parent": "r_shin" },
            { "name": "l_thigh", "parent": "pelvis" },
            { "name": "l_shin", "parent": "l_thigh" },
            { "name": "l_foot", "parent": "l_shin" }
        ]
        )"_json);

    for (const auto& bone : skeleton)
    {
        std::cout << bone << ": " << core::delimit(skeleton.children(bone), ", ") << std::endl;
    }
}

int main()
{
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::exception_ptr o;
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    return 0;
}
