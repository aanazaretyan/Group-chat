#include "catch.hpp"
#include "client.hpp"

TEST_CASE("Something works") { REQUIRE(not_random() == not_random()); }