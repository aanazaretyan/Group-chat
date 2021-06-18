#include "client.hpp"
#include <array>
#include <cmath>
#include <map>
#include <random>
#include <string>
#include <vector>
/**Проверка на простые числа. Смысл заключается в том,
что мы делим наше число на несколько первых простых чисел, до 1000.
Если поделилось, то наше число точно не простое*/
bool isPrime(int number) {
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
    for (int i : LowPrimes)
        if (number == i)
            return true;
    //Проверяем делимость нашего числа на небольшие простые числа
    for (int i : LowPrimes)
        if (number % i == 0)
            return false;
    return true;
}
/**Генерируется простое число.
Проверка на то, что число простое, есть в функции isPrime*/
int Generating_a_prime_number() {
    while (true) {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<int> uniform_dist(30, 500);
        int num = uniform_dist(e1);
        if (num % 2 == 0)
            num++;
        if (isPrime(num))
            return num;
    }
}
/**Проверяет, взаимно ли простые два f(n) и открытый ключ е
(или  два числа) на основе алгоритма Евклида*/
bool Mutual_prime_numbers_test(int f, int e) {
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
на основе функции Mutual_prime_numbers_test*/
int Generating_e(int f) {
    while (true) {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<int> uniform_dist(20, 300);
        int e = uniform_dist(e1);
        if (Mutual_prime_numbers_test(f, e) == true and e < f)
            return e;
    }
}
/**Расширенный алгоритм Евклида.
Функция взята из https://e-maxx.ru/algo/extended_euclid_algorithm*/
auto gcd(int a, int b, int &x, int &y) {
    int x1;
    int y1;
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    int d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}
/**Генерирует число d (закрытый ключ), так что e*d mod f == 1*/
int Generating_d(int f, int e) {
    while (true) {
        int x;
        int y;
        int g = gcd(e, f, x, y);
        if (g == 1) {
            x = (x % f + f) % f;
            return x;
        }
    }
}
/**Генерируются открытый и закрытый ключи*/
pairs keys() {
    int p;
    int q;
    int n;
    for (;;) {
        p = Generating_a_prime_number();
        q = Generating_a_prime_number();
        n = p * q;
        if (p != q and n > 94)
            break;
    }
    int f = (p - 1) * (q - 1);
    int e = Generating_e(f);
    int d = Generating_d(f, e);
    std::pair<long long, int> opkey = {e, n};
    std::pair<int, int> prkey = {d, n};
    pairs keys = {opkey, prkey};
    return keys;
}
/**Быстрое возведение числа в степень.
Функция взята из https://prog-cpp.ru/pow-mod/ */
int modexpop(int x, int y, int N) {
    if (y == 0)
        return 1;
    int z = modexpop(x, y / 2, N);
    if (y % 2 == 0)
        return (z * z) % N;
    return (x * z * z) % N;
};
/**Превращает исходную строку в массив (vector) так,
что каждая его ячейка заполняется значением ключа соответсвующего символа по
словарю*/
std::vector<int> code(std::string message) {
    std::map<char, int> map = {
        {'a', 40}, {'b', 37}, {'c', 48}, {'d', 38}, {'e', 26}, {'f', 10},
        {'g', 23}, {'h', 54}, {'i', 13}, {'j', 91}, {'k', 21}, {'l', 57},
        {'m', 33}, {'n', 78}, {'o', 49}, {'p', 59}, {'q', 20}, {'r', 70},
        {'s', 35}, {'t', 53}, {'u', 46}, {'v', 80}, {'w', 6},  {'x', 58},
        {'y', 50}, {'z', 67}, {'A', 66}, {'B', 63}, {'C', 44}, {'D', 87},
        {'E', 25}, {'F', 64}, {'G', 43}, {'H', 52}, {'I', 3},  {'J', 93},
        {'K', 16}, {'L', 85}, {'M', 30}, {'N', 71}, {'O', 9},  {'P', 92},
        {'Q', 29}, {'R', 51}, {'S', 45}, {'T', 81}, {'U', 42}, {'V', 69},
        {'W', 8},  {'X', 77}, {'Y', 11}, {'Z', 88}, {'0', 4},  {'1', 76},
        {'2', 31}, {'3', 55}, {'4', 34}, {'5', 68}, {'6', 2},  {'7', 73},
        {'8', 39}, {'9', 86}, {'!', 22}, {'@', 84}, {'#', 28}, {'$', 74},
        {'%', 5},  {'^', 94}, {'&', 18}, {'*', 82}, {'(', 17}, {')', 60},
        {'-', 7},  {'=', 89}, {'+', 24}, {'_', 61}, {'{', 14}, {'}', 75},
        {'[', 47}, {']', 83}, {';', 32}, {':', 65}, {'"', 27}, {'/', 62},
        {'?', 19}, {'<', 90}, {'>', 36}, {'.', 79}, {',', 12}, {'`', 56},
        {'~', 41}, {'|', 15}, {' ', 72},
    };
    std::vector<int> code_message;
    for (char i : message) {
        code_message.push_back(map[i]);
    }
    return code_message;
}
/**Превращает исходный массив в строку так,
что она заполняется постепенно (по каждой ячейке массива) заполняется
значением ключа соответсвующего символа по словарю*/
std::string decode(std::vector<int> code_message) {
    std::map<int, char> dmap = {
        {40, 'a'}, {37, 'b'}, {48, 'c'}, {38, 'd'}, {26, 'e'}, {10, 'f'},
        {23, 'g'}, {54, 'h'}, {13, 'i'}, {91, 'j'}, {21, 'k'}, {57, 'l'},
        {33, 'm'}, {78, 'n'}, {49, 'o'}, {59, 'p'}, {20, 'q'}, {70, 'r'},
        {35, 's'}, {53, 't'}, {46, 'u'}, {80, 'v'}, {6, 'w'},  {58, 'x'},
        {50, 'y'}, {67, 'z'}, {66, 'A'}, {63, 'B'}, {44, 'C'}, {87, 'D'},
        {25, 'E'}, {64, 'F'}, {43, 'G'}, {52, 'H'}, {3, 'I'},  {93, 'J'},
        {16, 'K'}, {85, 'L'}, {30, 'M'}, {71, 'N'}, {9, 'O'},  {92, 'P'},
        {29, 'Q'}, {51, 'R'}, {45, 'S'}, {81, 'T'}, {42, 'U'}, {69, 'V'},
        {8, 'W'},  {77, 'X'}, {11, 'Y'}, {88, 'Z'}, {4, '0'},  {76, '1'},
        {31, '2'}, {55, '3'}, {34, '4'}, {68, '5'}, {2, '6'},  {73, '7'},
        {39, '8'}, {86, '9'}, {22, '!'}, {84, '@'}, {28, '#'}, {74, '$'},
        {5, '%'},  {94, '^'}, {18, '&'}, {82, '*'}, {17, '('}, {60, ')'},
        {7, '-'},  {89, '='}, {24, '+'}, {61, '_'}, {14, '{'}, {75, '}'},
        {47, '['}, {83, ']'}, {32, ';'}, {65, ':'}, {27, '"'}, {62, '/'},
        {19, '?'}, {90, '<'}, {36, '>'}, {79, '.'}, {12, ','}, {56, '`'},
        {41, '~'}, {15, '|'}, {72, ' '}};
    std::string decode_message;
    for (auto i : code_message) {
        decode_message += dmap[i];
    }
    return decode_message;
}
/**Шифрование сообщения по алгоритму RSA*/
std::vector<int> encryption(std::string message, int e, int n) {
    std::vector<int> enc_message = code(message);
    for (std::size_t i = 0; i < enc_message.size(); i++) {
        enc_message[i] = modexpop(enc_message[i], e, n);
    }
    return enc_message;
}
/**Расшифрование сообщения по алгоритму RSA*/
std::string decryption(std::vector<int> enc_message, int d, int n) {
    std::vector<int> decryption_message;
    for (std::size_t i = 0; i < enc_message.size(); i++) {
        decryption_message.push_back(modexpop(enc_message[i], d, n));
    }
    std::string decode_message = decode(decryption_message);
    return decode_message;
}
/**Происходит генерация ключей такая, что сообщение впоследствии гарантированно
 и успешно зашифруется и расшифруется*/
pairs psedo_rsa_keys() {
    pairs v;
    for (;;) {
        std::string text =
            "qwertyuiop[]asdfghjkl;zxcvbnm,./"
            "1234567890!@#$%^&*()-_=+~`{}:?><QWERTYUIOPASDFGHJKLZXCVBNM";
        v = keys();
        auto enc_message =
            encryption(text, v.open_key.first, v.open_key.second);
        auto dec_message =
            decryption(enc_message, v.private_key.first, v.private_key.second);
        if (text == dec_message)
            break;
    }
    return v;
}