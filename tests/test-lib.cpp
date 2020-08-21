#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <project/lib.h>

TEST_CASE("Testing add()", "[add]") {
	REQUIRE(add(0, 0) == 0);
	REQUIRE(add(1, 1) == 2);
	REQUIRE(add(-1, -1) == -2);
}
