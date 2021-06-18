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
#include <random>

/**Шифрование сообщения по алгоритму RSA*/
std::vector<int> encryption(std::string, int, int);
std::string decryption(std::vector<int>, int, int);

struct pairs {
    std::pair<int, int>
        open_key;
    std::pair<int, int>
        private_key;
};

pairs keys();
int fast_exponentiation(int, int);
int Generating_a_prime_number();
int Generating_e(int);
int Generating_d(int, int);
int modexpop(int, int, int);           

pairs psedo_rsa_keys();                                                                      


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
