#include "catch.hpp"
#include "rsa.hpp"
#include <array>
#include <iostream>
#include <limits>
#include <string>

TEST_CASE("p and q test") {
    int p;
    int q;
    int n;
    for (;;) {
        p = Generating_a_prime_number();
        q = Generating_a_prime_number();
        n = p * q;
        if (p != q && n > 94)
            break;
    }
    REQUIRE(p != q);
};

TEST_CASE("Generating_e test") {
    int p;
    int q;
    for (;;) {
        p = Generating_a_prime_number();
        q = Generating_a_prime_number();
        if (p != q)
            break;
    }
    int f = (p - 1) * (q - 1);
    int e = Generating_e(f);
    bool a;
    if (1 < e && e < f)
        a = true;
    REQUIRE(a == true);
}

TEST_CASE("e and d mutual test") {
    int p;
    int q;
    for (;;) {
        p = Generating_a_prime_number();
        q = Generating_a_prime_number();
        if (p != q)
            break;
    }
    int f = (p - 1) * (q - 1);
    int e = Generating_e(f);
    int d = Generating_d(f, e);
    REQUIRE((e * d) % f == 1);
}

TEST_CASE("Modexpop test") {
    auto a = modexpop(5, 6, 8);
    REQUIRE(a == 1);
}
TEST_CASE("test 1") {
    // std::string text = "Hellqwerwr";
    std::string text = "Hello, world!";
    auto v = psedo_rsa_keys();
    auto enc_message = encryption(text, v.open_key.first, v.open_key.second);
    auto dec_message =
        decryption(enc_message, v.private_key.first, v.private_key.second);
    REQUIRE(text == dec_message);
}
TEST_CASE("code decode") {
    std::string v = "Albert";
    std::string d = decode(code(v));
    REQUIRE(v == d);
}
TEST_CASE("Parse deparse") {
    std::vector<int> v = {212, 1251, 23, 1};
    std::vector<int> d = deparse(parse(v));
    REQUIRE(v == d);
}
TEST_CASE("Final test") {
    // std::string text = "Hellqwerwr";
    std::string text =
        "qwertyuiop[]asdfghjkl;zxcvbnm,./"
        "1234567890!@#$%^&*()-_=+~`{}:?><QWERTYUIOPASDFGHJKLZXCVBNM";
    auto v = psedo_rsa_keys();
    auto enc_message = encryption(text, v.open_key.first, v.open_key.second);
    auto dec_message =
        decryption(enc_message, v.private_key.first, v.private_key.second);
    REQUIRE(text == dec_message);
}
