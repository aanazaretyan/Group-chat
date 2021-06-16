#pragma once
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <vector>
#include <memory>

int not_random();

class session : public std::enable_shared_from_this<session> {

  public:
    explicit session(boost::asio::io_context &io_context, boost::asio::ip::tcp::socket t_socket);
    static std::vector<std::shared_ptr<session>> users;
    void write();
    void read();
    std::string ID;

  private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::strand<boost::asio::io_context::executor_type> strand;
};
