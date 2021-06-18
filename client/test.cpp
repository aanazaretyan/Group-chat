#include "catch.hpp"
#include "client.hpp"
#include <array>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <random>

TEST_CASE("First test") {
    int p;
    int q;
    int n;
    for (;;){
        p = Generating_a_prime_number();
        q = Generating_a_prime_number();
        n = p * q;
        if (p!=q && n > 94) break;

    }
    REQUIRE(p!=q ); };
TEST_CASE("1 < e < f test") {
    int p;
    int q;
    for (;;){
        p = Generating_a_prime_number();
        q = Generating_a_prime_number();
        if (p!=q) break;

    }
    int f = (p - 1) * (q - 1);
    int e = Generating_e(f);
    bool a;
    if (1 < e && e < f)  a = true;
    REQUIRE( a  == true  );
}   
TEST_CASE("e and d mutual test"){
    int p;
    int q;
    for (;;){
        p = Generating_a_prime_number();
        q = Generating_a_prime_number();
        if (p!=q) break;

    }
    int f = (p - 1) * (q - 1);
    int e = Generating_e(f);
    int d = Generating_d(f, e);
    REQUIRE((e * d) % f == 1 );

}    
TEST_CASE("Second test") {
    //std::string text = "Hellqwerwr";
    std::string text = "qwertyuiop[]asdfghjkl;zxcvbnm,./1234567890!@#$%^&*()-_=+~`{}:?><QWERTYUIOPASDFGHJKLZXCVBNM";
    auto v = psedo_rsa_keys();
    auto enc_message = encryption(text, v.open_key.first, v.open_key.second);  
    auto dec_message = decryption(enc_message,v.private_key.first,v.private_key.second);  
    REQUIRE(text == dec_message);
}
TEST_CASE("Simple test") {    
auto a = modexpop(5, 6, 8);
REQUIRE(a == 1);
}
