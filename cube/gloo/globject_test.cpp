//=============================================================================
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//=============================================================================

#include <catch2/catch.hpp>

#include "cube/gloo/globject.hpp"

struct test_globject : cube::gloo::globject<test_globject>
{
    test_globject()
        : create_called(0U)
        , enable_called(0U)
        , disable_called(0U)
        , destroy_called(0U)
    {
    }

    unsigned create_called;
    unsigned enable_called;
    unsigned disable_called;
    unsigned destroy_called;

private:
    friend class cube::gloo::globject<test_globject>;

    void create() { ++create_called; }
    void enable() { ++enable_called; }
    void disable() { ++disable_called; }
    void destroy() { ++destroy_called; }
};

TEST_CASE("cube::gloo::globject state test", "[globject]")
{
    test_globject obj;

    REQUIRE(obj.state() == test_globject::constructed);
    REQUIRE(obj.create_called == 0U);
    REQUIRE(obj.enable_called == 0U);
    REQUIRE(obj.disable_called == 0U);
    REQUIRE(obj.destroy_called == 0U);

    obj.activate();

    REQUIRE(obj.state() == test_globject::enabled);
    REQUIRE(obj.create_called == 1U);
    REQUIRE(obj.enable_called == 1U);
    REQUIRE(obj.disable_called == 0U);
    REQUIRE(obj.destroy_called == 0U);

    obj.deactivate();

    REQUIRE(obj.state() == test_globject::idle);
    REQUIRE(obj.create_called == 1U);
    REQUIRE(obj.enable_called == 1U);
    REQUIRE(obj.disable_called == 1U);
    REQUIRE(obj.destroy_called == 0U);

    obj.activate();

    REQUIRE(obj.state() == test_globject::enabled);
    REQUIRE(obj.create_called == 1U);
    REQUIRE(obj.enable_called == 2U);
    REQUIRE(obj.disable_called == 1U);
    REQUIRE(obj.destroy_called == 0U);

    obj.dispose();

    REQUIRE(obj.state() == test_globject::dead);
    REQUIRE(obj.create_called == 1U);
    REQUIRE(obj.enable_called == 2U);
    REQUIRE(obj.disable_called == 2U);
    REQUIRE(obj.destroy_called == 1U);
}
