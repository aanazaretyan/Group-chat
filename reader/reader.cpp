#include <iostream>
#include "reader.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <charconv>
#include <iostream>
#include <memory>
#include <string_view>
#include <thread>
#include <sstream>


using boost::asio::ip::tcp;
// TODO: add tests
// TODO: add HTTP
// TODO: add json

template <typename Class, typename Function>
auto delegate(std::shared_ptr<Class> ptr, Function fun) {
    // return [ptr = std::move(ptr), fun]() {
    return[ptr, fun]<typename... Args>(Args && ...arg) {
        return (ptr.get()->*fun)(std::forward<Args>(arg)...);
    };
}

session::session(boost::asio::io_context &io_context, tcp::socket t_socket)
    : socket(std::move(t_socket)), strand(io_context.get_executor()) {}

void session::read() {
    auto self(shared_from_this());
    boost::asio::spawn(strand, [this, self](boost::asio::yield_context yield) {
        try {
            std::string ID;
            std::cout << "Enter your name: ";
            std::getline(std::cin, ID);
            boost::asio::async_write(
                socket, boost::asio::buffer("r" + ID + "\n"), yield);
            for (;;) {
                std::string data;
                boost::asio::async_read_until(
                    socket, boost::asio::dynamic_buffer(data), "\n", yield);
                std::cout << data;
            }
        } catch (std::exception &e) {
            socket.close();
            std::cerr << "Exception: " << e.what() << "\n";
        }
    });
}

std::vector<std::shared_ptr<session>> session::users;


int main() {
    try {
        boost::asio::io_context io_context;
        boost::system::error_code ec;
        tcp::socket socket(io_context);
        std::string addr = "127.0.0.1";
        unsigned short port = 1234;
        //std::string addr;
        //unsigned short port;
        //std::cout << "Enter IP-address and port:" << std::endl;
        //std::cin >> addr >> port;
        boost::asio::ip::tcp::endpoint serv_addr(
            boost::asio::ip::make_address(addr, ec), port);
        socket.connect(serv_addr, ec);

        if (!ec) {
            socket.wait(socket.wait_read);
            size_t bytes = socket.available();
            if (bytes > 0) {
                std::vector<char> data(bytes);
                socket.read_some(boost::asio::buffer(data.data(), data.size()), ec);
                for (auto c : data) {
                    std::cout << c;
                }
            }
            auto new_session =
                std::make_shared<session>(io_context, std::move(socket));
            new_session->read();
        }
        else {
            std::cerr << ec << "\n";
        }

        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    system("pause");
    return 0;
}