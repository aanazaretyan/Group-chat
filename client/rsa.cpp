#include "client.hpp"
#include <array>
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <botan/rng.h>
#include <botan/system_rng.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <new>
#include <random>
#include <stdint.h>
#include <string>
#include <vector>
/**Проверка на простые числа. Смысл заключается в том,
что мы делим наше число на несколько первых простых чисел, до 1000.
Если поделилось, то наше число точно не простое*/
bool isPrime(long int number) {
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
    for (long int i : LowPrimes)
        if (number == i)
            return true;
    //Проверяем делимость нашего числа на небольшие простые числа
    for (long int i : LowPrimes)
        if (number % i == 0)
            return false;
    return true;
}

/**Генерируется простое число.
Проверка на то, что число простое, есть в функции isPrime*/
long int Generating_a_prime_number() {
    while (true) {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<long int> uniform_dist(1,
                                                              100);
        long int num = uniform_dist(e1);                                                      
        if (num % 2 == 0) num++;
        if (isPrime(num))
            return num;
    }
}
/**Проверяет, взаимно ли простые два f(n) и открытый ключ е
(или  два числа) на основе алгоритма Евклида*/
bool Mutual_prime_numbers_test(long int f,
                               long int e) {
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
long int Generating_e(long int f) {
    while (true) {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<long int> uniform_dist(1,
                                                              100);
        long int e = uniform_dist(e1);
        if (Mutual_prime_numbers_test(f, e) == true)
            return e;
    }
}
/**Расширенный алгоритм Евклида.
Функция взята из https://e-maxx.ru/algo/extended_euclid_algorithm*/
auto gcd(long int a, long int b,
         long int &x, long int &y) {
    long int x1;
    long int y1;
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    long int d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}
/**Генерирует число d (закрытый ключ), так что e*d mod f == 1*/
long int Generating_d(long int f,
                                            long int e) {
    while (true) {
        long int x;
        long int y;
        long int g = gcd(e, f, x, y);
        if (g == 1) {
            x = (x % f + f) % f;
            return x;
        }
    }
}
//Создается структура для хранения пар открытого и закрытого ключей

/**Генерируются открытый и закрытый ключи*/
pairs keys() {
    long int p = Generating_a_prime_number() ;
    long int q = Generating_a_prime_number();
    long int n = p * q;
    long int f = (p - 1) * (q - 1);
    long int e = Generating_e(f);
    long int d = Generating_d(f, e);
    std::pair<long long, long int> opkey = {e, n};
    std::pair<long int, long int>
        prkey = {d, n};
    pairs keys = {opkey, prkey};
    return keys;
}
/**Быстрое возведение числа в степень.
Функция взята из http://mindhalls.ru/binary-power-in-c-cpp/*/
long int fast_exponentiation(long int dec,
                    long int d) {
    long int result = 1;
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
long int modexpop(long int x,
                                        long int y,
                                        long int N) {
    if (y == 0)
        return 1;
    long int z = modexpop(x, y / 2, N);
    if (y % 2 == 0)
        return (z * z) % N;
    return (x * z * z) % N;
};
 
/**Шифрование сообщения по алгоритму RSA*/
std::vector<int> encryption(std::string message, long int e,
                       long int n) {
    std::vector<int> emessage;
    std::map<char, long int> map  = {{'H',1}, {'e',2}};
    //int g = map ['H'];
    for (char i:message){
        int l = map [i];
        l = modexpop(l, e, n);
        emessage.push_back(l);
    }
    return emessage;
}

/**Расшифрование сообщения по алгоритму RSA*/
std::string decryption(std::vector<int> enc_message,
                       long int d,
                       long int n) {
std::string dec_message;
std::map<int, char> dmap = {{1,'H'}, {2,'e'}};
    for (int i:enc_message){
        i  = modexpop(i, d, n);
        dec_message+=dmap[i];
    }
return dec_message;
}