#include "client.hpp"
#include <array>
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <botan/rng.h>
#include <botan/system_rng.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <new>
#include <random>
#include <stdint.h>
/**Проверка на простые числа. Смысл заключается в том,
что мы делим наше число на несколько первых простых чисел, до 1000.
Если поделилось, то наше число точно не простое*/
bool isPrime(boost::multiprecision::cpp_int number) {
    if (number < 2)
        return false; // отбросим числа 0, 1 и отрицательные числа, они не
                      // являются простыми
    auto LowPrimes = {
        2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,
        47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107,
        109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
        191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263,
        269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
        353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433,
        439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521,
        523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613,
        617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701,
        709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
        811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887,
        907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};
    //Проверяем, есть ли наше число в LowPrimes
    for (int i : LowPrimes)
        if (number == i)
            return true;
    //Проверяем делимость нашего числа на небольшие простые числа
    for (int i : LowPrimes)
        if (number % i == 0)
            return false;
}
/**Генерируется простое число размера 1024 бит.
Проверка на то, что число простое, есть в функции isPrime*/
std::array<uint8_t, 128> Generating_a_prime_number() {
    while (true) {
        std::array<uint8_t, 128> number;
        auto &generator = Botan::system_rng();
        generator.randomize(number.data(), number.size());
        boost::multiprecision::cpp_int num;
        import_bits(num, number.begin(), number.end());
        if (isPrime(num) == true)
            return number;
    }
}
/**Проверяет, взаимно ли простые два f(n) и открытый ключ е
(или  два числа) на основе алгоритма Евклида*/
bool Mutual_prime_numbers_test(boost::multiprecision::cpp_int f,
                               boost::multiprecision::cpp_int e) {
    while (f != 0 and e != 0) {
        if (f > e)
            f %= e;
        else
            e %= f;
    }
    if ((f + e) == 1)
        return true;
    else
        return false;
}
/**Генерируется число e (открытый ключ), проверяя, взаимно ли простое оно
с помощью функции на основе функции Mutual_prime_numbers_test*/
long long Generating_e(boost::multiprecision::cpp_int f) {
    while (true) {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<long long> uniform_dist(1000000000,
                                                              100000000000);
        auto e = uniform_dist(e1);
        if (Mutual_prime_numbers_test(f, e) == true)
            return e;
    }
}
/**Расширенный алгоритм Евклида.
Функция взята из https://e-maxx.ru/algo/extended_euclid_algorithm*/
int gcd(boost::multiprecision::cpp_int a, boost::multiprecision::cpp_int b,
        boost::multiprecision::cpp_int &x, boost::multiprecision::cpp_int &y) {
    boost::multiprecision::cpp_int x1;
    boost::multiprecision::cpp_int y1;
    int d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}
/**Генерирует число d (закрытый ключ), так что e*d mod f == 1*/
boost::multiprecision::cpp_int Generating_d(boost::multiprecision::cpp_int f,
                                            long long e) {
    while (true) {
        boost::multiprecision::cpp_int x;
        boost::multiprecision::cpp_int y;
        int g = gcd(e, f, x, y);
        if (g == 1) {
            x = (x % f + f) % f;
            return x;
        }
    }
}
//Создается структура для хранения пар открытого и закрытого ключей
struct pairs {
    std::pair<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int>
        open_key;
    std::pair<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int>
        private_key;
};
/**Генерируются открытый и закрытый ключи*/
pairs open_key() {
    std::array<uint8_t, 128> p = Generating_a_prime_number();
    std::array<uint8_t, 128> q = Generating_a_prime_number();
    boost::multiprecision::cpp_int pint;
    boost::multiprecision::cpp_int qint;
    import_bits(pint, p.begin(), p.end());
    import_bits(qint, q.begin(), q.end());
    boost::multiprecision::cpp_int n = pint * qint;
    boost::multiprecision::cpp_int f = (pint - 1) * (qint - 1);
    long long e = Generating_e(f);
    boost::multiprecision::cpp_int d = Generating_d(f, e);
    std::pair<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int>
        opkey = {e, n};
    std::pair<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int>
        prkey = {d, n};
    pairs keys = {opkey, prkey};
    return keys;
}
/**Шифрование сообщения по алгоритму RSA*/
std::string encryption(std::string message, long long e,
                       boost::multiprecision::cpp_int n) {
    std::string enc_message;
    boost::multiprecision::cpp_int enc;
    import_bits(enc, message.begin(), message.end());
    enc = pow(enc, e) % n;
    export_bits(enc, std::back_inserter(enc_message), 8);
    return enc_message;
}
/**Быстрое возведение числа в степень.
Функция взята из http://mindhalls.ru/binary-power-in-c-cpp/*/
boost::multiprecision::cpp_int fast_exponentiation(boost::multiprecision::cpp_int dec,
                    boost::multiprecision::cpp_int d) {
    boost::multiprecision::cpp_int result = 1;
    while (d) {
        if (d % 2 == 0) {
            d /= 2;
            dec *= dec;
        } else {
            d--;
            result *= dec;
        }
    }
    return result;
}
/**Расшифрование сообщения по алгоритму RSA*/
std::string decryption(std::string enc_message,
                       boost::multiprecision::cpp_int d,
                       boost::multiprecision::cpp_int n) {
    std::string dec_message;
    boost::multiprecision::cpp_int dec;
    import_bits(dec, enc_message.begin(), enc_message.end());
    dec = fast_exponentiation(dec, d);
    export_bits(dec, std::back_inserter(dec_message), 8);
    return dec_message;
}