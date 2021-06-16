#include "client.hpp"
#include <cmath>
#include <ctime>
#include <random>

/*Дело в том, что тест Миллера — Рабина хоть и относительно быстр.
Этой скорости может быть недостаточно.
Поэтому существуют ещё один метод проверки на простоту,
который можно использовать до теста Миллера - Рабина.
Подробнее ниже.*/
bool isPrime(int number) {
    if (number < 2)
        return false; // 0, 1 и отрицательные числа не являются простыми
    /*Смысл заключается в том, что мы делим наше число
  На несколько первых простых чисел, до 1000
  Если поделилось, то наше число точно не простое*/
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
    //Смотрим есть ли наше число в LowPrimes
    for (int i : LowPrimes)
        if (number == i)
            return true;
    //Проверяем делимость нашего числа на небольшие простые числа
    for (int i : LowPrimes)
        if (number % i == 0)
            return false;
}
void Generating_a_prime_number() {
    //Генерируется простое число размера 64 бит//
    while (1) {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<long long> uniform_dist(
            100000000000000000, 1000000000000000000);
        auto number = uniform_dist(e1);
        if (isPrime(number) == true)
            return;
    }
}
bool Mutual_prime_numbers_test(int z, int e) {
    //Проверяет, взаимно ли простые два принимаемых функцией на основе
    //алгоритма//
    while (z != 0 and e != 0) {
        if (z > e)
            z %= e;
        else
            e %= z;
    }
    if ((z + e) == 1)
        return true;
    else
        return false;
}
auto Generating_e(int n) {
    //Подбирает число E, проверяя взаимно ли простое оно с помощью функции
    // CoprimeTest//
    while (true) {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<long long> uniform_dist(1000000000,
                                                              100000000000);
        auto e = uniform_dist(e1);
        if (Mutual_prime_numbers_test(n, e) == true)
            return e;
    }
}
void newkeys() { // Генерирует открытый
}
std::string encryption(std::string message, long long e, int n) {
    std::string enc_message;
    /*Функция шифрует сообщение по алгоритму RSA*/
    for (char l : message) {
        int i = l;
        char enc_l = pow(i, e);
        enc_l = enc_l % n;
        enc_message += enc_l;
    }
    return enc_message;
}