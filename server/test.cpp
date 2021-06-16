#include "catch.hpp"
#include "server.hpp"

TEST_CASE("Something works") { REQUIRE(not_random() == not_random()); }