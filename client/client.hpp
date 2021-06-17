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
std::vector<int> encryption(std::string, long int, long int);
std::string decryption(std::vector<int>, long int,
                       long int);
struct pairs {
    std::pair<long int, long int>
        open_key;
    std::pair<long int, long int>
        private_key;
};
pairs keys();
long int
    fast_exponentiation(long int,
                        long int);
long int Generating_a_prime_number();
long int Generating_e(long int);
long int Generating_d(long int,
                                            long int);
long int modexpop(long int , long int , long int);                                                                                 
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
