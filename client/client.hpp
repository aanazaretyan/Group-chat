#pragma once
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <memory>
#include <vector>

int not_random();
/**Шифрование сообщения по алгоритму RSA*/
std::string encryption(std::string, long long, boost::multiprecision::cpp_int);
std::string decryption(std::string, boost::multiprecision::cpp_int,
                       boost::multiprecision::cpp_int);
struct pairs {
    std::pair<long long, boost::multiprecision::cpp_int>
        open_key;
    std::pair<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int>
        private_key;
};
pairs open_key();
boost::multiprecision::cpp_int
    fast_exponentiation(boost::multiprecision::cpp_int,
                        boost::multiprecision::cpp_int);
std::array<uint8_t, 128> Generating_a_prime_number();
long long Generating_e(boost::multiprecision::cpp_int);
boost::multiprecision::cpp_int Generating_d(boost::multiprecision::cpp_int,
                                            long long);
class session : public std::enable_shared_from_this<session> {
  public:
    explicit session(boost::asio::io_context &io_context,
                     boost::asio::ip::tcp::socket t_socket);
    void write();
    static std::vector<std::shared_ptr<session>> users;

  private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::strand<boost::asio::io_context::executor_type> strand;
};
