//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include "catch.hpp"

#include "cube/mat.hpp"

TEST_CASE("cube::mat::eye test", "[mat]")
{
    const auto m = cube::mat4::eye();

    REQUIRE((m[{0, 0}]) == Approx(1.0f));
    REQUIRE((m[{1, 1}]) == Approx(1.0f));
    REQUIRE((m[{2, 2}]) == Approx(1.0f));
    REQUIRE((m[{3, 3}]) == Approx(1.0f));

    REQUIRE((m[{0, 1}]) == Approx(0.0f));
    REQUIRE((m[{0, 2}]) == Approx(0.0f));
    REQUIRE((m[{0, 3}]) == Approx(0.0f));

    REQUIRE((m[{1, 0}]) == Approx(0.0f));
    REQUIRE((m[{1, 2}]) == Approx(0.0f));
    REQUIRE((m[{1, 3}]) == Approx(0.0f));

    REQUIRE((m[{2, 0}]) == Approx(0.0f));
    REQUIRE((m[{2, 1}]) == Approx(0.0f));
    REQUIRE((m[{2, 3}]) == Approx(0.0f));

    REQUIRE((m[{3, 0}]) == Approx(0.0f));
    REQUIRE((m[{3, 1}]) == Approx(0.0f));
    REQUIRE((m[{3, 2}]) == Approx(0.0f));
}
