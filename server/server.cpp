//
// echo_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <charconv>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>
#include <utility>
#include <chrono>
#include "lib.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;
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

    explicit session::session(boost::asio::io_context& io_context, tcp::socket t_socket)
        : socket(std::move(t_socket)), timer(io_context),
        strand(io_context.get_executor()) {}

    void session:: go() {
        auto self(shared_from_this());
        boost::asio::spawn(strand, [this,
            self](boost::asio::yield_context yield) {
                try {
                    // std::array<char, 128> data;
                    for (;;) { // while (true)
                        std::string data;
                        timer.expires_from_now(std::chrono::minutes(10));
                        size_t m = boost::asio::async_read_until(
                            socket, boost::asio::dynamic_buffer(data), "\n", yield);
                        std::cout << data;
                        // https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/overview/core/line_based.html
                    }
                }
                catch (std::exception& e) {
                    socket.close();
                    timer.cancel();
                }
            });

        boost::asio::spawn(strand, delegate(self, &session::timer_callback));
    }

    void session:: timer_callback(boost::asio::yield_context yield) {
        while (socket.is_open()) {
            boost::system::error_code ignored_ec;
            timer.async_wait(yield[ignored_ec]);
            if (timer.expires_from_now() <= std::chrono::seconds(0))
                socket.close();
        }
    }

int main(int argc, char* argv[]) {
    try {
        unsigned short port = 1234;
        if (argc > 1)
            port = static_cast<unsigned short>(std::atoi(argv[1]));
        // if (argc != 2) {
        //     std::cerr << "Usage: echo_server <port>\n";
        //     return 1;
        // }

        boost::asio::io_context io_context;

        boost::asio::spawn(io_context, [&](boost::asio::yield_context yield) {
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

            while (true) {
                boost::system::error_code ec;
                tcp::socket socket(io_context);
                acceptor.async_accept(socket, yield[ec]);
                if (!ec) {
                    std::cout << "New connection" << std::endl;
                    std::string message = "Hi, you are connected!\n";
                    socket.write_some(
                        boost::asio::buffer(message.data(), message.size()), ec);
                    auto sess = std::make_shared<session>(io_context, std::move(socket));
                    //session::users.push_back(sess);
                    sess -> go();
                }
                else {
                    std::cerr << ec << "\n";
                }
            }
            });

        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}