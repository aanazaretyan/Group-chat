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

boost::multiprecision::cpp_int num1 = 5;
boost::multiprecision::cpp_int num2 = 7;
boost::multiprecision::cpp_int numL = int(pow(5, 7));
TEST_CASE("Fast_exponentiation") {
    REQUIRE(numL == fast_exponentiation(num1, num2));
}
TEST_CASE("First test") {
    std::array<uint8_t, 128> p = Generating_a_prime_number();
    std::array<uint8_t, 128> q = Generating_a_prime_number();
    boost::multiprecision::cpp_int pint;
    boost::multiprecision::cpp_int qint;
    import_bits(pint, p.begin(), p.end());
    import_bits(qint, q.begin(), q.end()); 
    REQUIRE(pint!=qint); };
TEST_CASE("Second test") {
    std::string text = "He";
    std::cout << "Open_key start\n";
    auto v = open_key();
    auto enc_message = encryption(text, v.open_key.first, v.open_key.second);
    std::cout<<enc_message<<"\n";  
    auto dec_message = decryption(enc_message,v.private_key.first,v.private_key.second);
    std::cout<<dec_message<<"\n";  
    //REQUIRE(text == decryption(enc_message, v.private_key.first, v.private_key.second));
}

