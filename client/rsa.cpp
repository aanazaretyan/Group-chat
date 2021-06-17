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
bool isPrime(unsigned long long number) {
    if (number < 2)
        return false; // отбросим числа 0, 1 и отрицательные числа, они не
                      // являются простыми
    auto LowPrimes = {
        3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
        53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109,
        113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191,
        193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269,
        271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353,
        359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439,
        443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523,
        541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617,
        619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709,
        719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
        821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907,
        911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};
    //Проверяем, есть ли наше число в LowPrimes
    for (unsigned long long i : LowPrimes)
        if (number == i)
            return true;
    //Проверяем делимость нашего числа на небольшие простые числа
    for (unsigned long long i : LowPrimes)
        if (number % i == 0)
            return false;
    return true;
}

/**Генерируется простое число.
Проверка на то, что число простое, есть в функции isPrime*/
unsigned long long Generating_a_prime_number() {
    while (true) {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<unsigned long long> uniform_dist(1000000000,
                                                              100000000000);
        unsigned long long num = uniform_dist(e1);                                                      
        if (num % 2 == 0) num++;
        if (isPrime(num))
            return num;
    }
}
/**Проверяет, взаимно ли простые два f(n) и открытый ключ е
(или  два числа) на основе алгоритма Евклида*/
bool Mutual_prime_numbers_test(unsigned long long f,
                               unsigned long long e) {
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
unsigned long long Generating_e(unsigned long long f) {
    while (true) {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<unsigned long long> uniform_dist(1000000000,
                                                              100000000000);
        unsigned long long e = uniform_dist(e1);
        if (Mutual_prime_numbers_test(f, e) == true)
            return e;
    }
}
/**Расширенный алгоритм Евклида.
Функция взята из https://e-maxx.ru/algo/extended_euclid_algorithm*/
auto gcd(unsigned long long a, unsigned long long b,
         unsigned long long &x, unsigned long long &y) {
    unsigned long long x1;
    unsigned long long y1;
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    unsigned long long d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}
/**Генерирует число d (закрытый ключ), так что e*d mod f == 1*/
unsigned long long Generating_d(unsigned long long f,
                                            unsigned long long e) {
    while (true) {
        unsigned long long x;
        unsigned long long y;
        unsigned long long g = gcd(e, f, x, y);
        if (g == 1) {
            x = (x % f + f) % f;
            return x;
        }
    }
}
//Создается структура для хранения пар открытого и закрытого ключей

/**Генерируются открытый и закрытый ключи*/
pairs keys() {
    unsigned long long p = Generating_a_prime_number() ;
    unsigned long long q = Generating_a_prime_number();
    unsigned long long n = p * q;
    unsigned long long f = (p - 1) * (q - 1);
    unsigned long long e = Generating_e(f);
    unsigned long long d = Generating_d(f, e);
    std::pair<long long, unsigned long long> opkey = {e, n};
    std::pair<unsigned long long, unsigned long long>
        prkey = {d, n};
    pairs keys = {opkey, prkey};
    return keys;
}
/**Быстрое возведение числа в степень.
Функция взята из http://mindhalls.ru/binary-power-in-c-cpp/*/
unsigned long long fast_exponentiation(unsigned long long dec,
                    unsigned long long d) {
    unsigned long long result = 1;
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
unsigned long long modexpop(int x,
                                        unsigned long long y,
                                        unsigned long long N) {
    if (y == 0)
        return 1;
    unsigned long long z = modexpop(x, y / 2, N);
    if (y % 2 == 0)
        return (z * z) % N;
    return (x * z * z) % N;
}
/**Шифрование сообщения по алгоритму RSA*/
std::string encryption(std::string message, unsigned long long e,
                       unsigned long long n) {
    std::string enc_message;
    for (char i:message){
        int l;
        l = i;
        l = modexpop(l, e, n); 
        enc_message+=char(l);
    }
    return enc_message;
}
/**Расшифрование сообщения по алгоритму RSA*/
std::string decryption(std::string enc_message,
                       unsigned long long d,
                       unsigned long long n) {
    for (char i:enc_message){
        int l;
        l = i;
        l = modexpop(l, d, n);
        enc_message+=char(l);
    }
    return enc_message;
}