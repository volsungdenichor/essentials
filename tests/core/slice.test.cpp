#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("views::take")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | sq::take(3)) == vec(2, 4, 5));
}

TEST_CASE("views::drop")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | sq::drop(3)) == vec(7, 3, 1, 2));
}

TEST_CASE("views::take_back")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | sq::take_back(3)) == vec(3, 1, 2));
}

TEST_CASE("views::drop_back")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | sq::drop_back(3)) == vec(2, 4, 5, 7));
}

TEST_CASE("views::slice")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | sq::slice(2, 4)) == vec(5, 7));
    REQUIRE((vect | sq::slice(3, nil)) == vec(7, 3, 1, 2));
    REQUIRE((vect | sq::slice(-3, nil)) == vec(3, 1, 2));
    REQUIRE((vect | sq::slice(nil, 3)) == vec(2, 4, 5));
    REQUIRE((vect | sq::slice(nil, -3)) == vec(2, 4, 5, 7));
    REQUIRE((vect | sq::slice(nil, nil)) == vec(2, 4, 5, 7, 3, 1, 2));
    REQUIRE((vect | sq::slice(4, 1)) == empty_vec<int>());
    REQUIRE((vect | sq::slice(4, 4)) == empty_vec<int>());
    REQUIRE((vect | sq::slice(40, 50)) == empty_vec<int>());

}
