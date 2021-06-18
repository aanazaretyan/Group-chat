#pragma once
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <random>
#include <vector>

/**Генерируется простое число.
Проверка на то, что число простое, есть в функции isPrime*/
int Generating_a_prime_number();

/**Генерируется число e (открытый ключ), проверяя, взаимно ли простое оно
на основе функции Mutual_prime_numbers_test*/
int Generating_e(int);

/**Генерирует число d (закрытый ключ), так что e*d mod f == 1*/
int Generating_d(int, int);

/**Быстрое возведение числа в степень.
Функция взята из https://prog-cpp.ru/pow-mod/ */
int modexpop(int, int, int);

/** Структура из двух пар ключей*/
struct pairs {
    std::pair<int, int> open_key;
    std::pair<int, int> private_key;
};

/**Генерируются открытый и закрытый ключи*/
pairs keys();

/**Происходит генерация ключей такая, что сообщение впоследствии гарантированно
 и успешно зашифруется и расшифруется*/
pairs psedo_rsa_keys();

/**Кодировка массива чисел*/
std::vector<int> code(std::string);

/**Декодировка массива чисел*/
std::string decode(std::vector<int>);

/**Шифрование сообщения по алгоритму RSA*/
std::vector<int> encryption(std::string, int, int);

/**Расшифрование сообщения по алгоритму RSA*/
std::string decryption(std::vector<int>, int, int);

/**Функция перебирает массив и составляет строку*/
std::string parse(std::vector<int>);

/**Функция перебирает строку и составляет массив*/
std::vector<int> deparse(std::string);

/**Объекты этого класса - отдельные участники сетевого взаимодействия */
class session : public std::enable_shared_from_this<session> {
  public:
    explicit session(boost::asio::io_context &io_context,
                     boost::asio::ip::tcp::socket t_socket);
    void write();
    static std::vector<std::shared_ptr<session>> users;
    std::pair<int, int> open_key;

  private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::strand<boost::asio::io_context::executor_type> strand;
};
