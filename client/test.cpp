#include "client.hpp"
#include "catch.hpp"
#include <string>
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <botan/rng.h>
#include <botan/system_rng.h>
boost::multiprecision::cpp_int num1 = 5;
boost::multiprecision::cpp_int num2 = 7;
boost::multiprecision::cpp_int numL = int (pow(5,7));
TEST_CASE("Fast_exponentiation") { REQUIRE (numL == fast_exponentiation(num1, num2) ); }
std::string text = "Hello, world!";
//pairs v = open_key(); 
//TEST_CASE("First test") { REQUIRE(text==decryption(encryption())); }
//std::array<uint8_t, 128> p = Generating_a_prime_number();
//std::array<uint8_t, 128> q = Generating_a_prime_number();
//TEST_CASE("Second test") { REQUIRE(p!=q); };