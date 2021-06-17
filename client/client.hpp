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

int not_random();
/**Шифрование сообщения по алгоритму RSA*/
std::string encryption(std::string, unsigned long long, unsigned long long);
std::string decryption(std::string, unsigned long long,
                       unsigned long long );
struct pairs {
    std::pair<unsigned long long, unsigned long long>
        open_key;
    std::pair<unsigned long long, unsigned long long>
        private_key;
};
pairs keys();
unsigned long long
    fast_exponentiation(unsigned long long,
                        unsigned long long);
unsigned long long Generating_a_prime_number();
unsigned long long Generating_e(unsigned long long);
unsigned long long Generating_d(unsigned long long,
                                            long long);
unsigned long long modexpop(unsigned long long ,
                                        long long ,
                                        unsigned long long);
unsigned long long modexppr(unsigned long long x,
                                        unsigned long long y,
                                        unsigned long long N);                                                                                   
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
