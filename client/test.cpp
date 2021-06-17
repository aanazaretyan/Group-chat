#include "catch.hpp"
#include "client.hpp"
#include <array>
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <botan/rng.h>
#include <botan/system_rng.h>
#include <iostream>
#include <limits>
#include <new>
#include <random>
#include <stdint.h>
#include <string>
#include <random>

unsigned long long num1 = 5;
unsigned long long num2 = 7;
unsigned long long numL = int(pow(5, 7));
TEST_CASE("Fast_exponentiation") {
    REQUIRE(numL == fast_exponentiation(num1, num2));
}
TEST_CASE("First test") {
    unsigned long long p = Generating_a_prime_number();
    unsigned long long q = Generating_a_prime_number();
    REQUIRE(p!=q); };
TEST_CASE("Second test") {
    std::string text = "He";
    auto v = keys();
    auto enc_message = encryption(text, v.open_key.first, v.open_key.second);  
    auto dec_message = decryption(enc_message,v.private_key.first,v.private_key.second);  
    REQUIRE(text == dec_message);
}


