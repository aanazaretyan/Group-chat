#include "client.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <new>
#include <random>
#include <stdint.h>

/**Проверка на простые числа*/
bool isPrime(boost::multiprecision::cpp_int number) {
    using namespace boost::multiprecision;
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
/**Генерируется простое число размера 1024 бит**/
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
bool Mutual_prime_numbers_test(boost::multiprecision::cpp_int f, boost::multiprecision::cpp_int e) {
    //Проверяет, взаимно ли простые два принимаемых функцией на основе
    //алгоритма//
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
long long Generating_e(boost::multiprecision::cpp_int f) {
    //Подбирает число E, проверяя взаимно ли простое оно с помощью функции
    // CoprimeTest//
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
long long Generating_d(boost::multiprecision::cpp_int f, long long e) {
    //Подбирает число D, такое что E*D mod f == 1
    while (true) {
       /// long long d = pow(e, -1) % f;
    }
}
// Генерирует открытый и закрытый 
void newkeys() {
    std::array<uint8_t, 128> p = Generating_a_prime_number();
    std::array<uint8_t, 128> q = Generating_a_prime_number();
    boost::multiprecision::cpp_int pint;
    boost::multiprecision::cpp_int qint;
    import_bits(pint, p.begin(), p.end());
    import_bits(qint, q.begin(), q.end());
    boost::multiprecision::cpp_int n = pint * qint ;
    boost::multiprecision::cpp_int f = (pint-1) * (qint-1);
    long long e = Generating_e(f);




    

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
    std::string de;
}
