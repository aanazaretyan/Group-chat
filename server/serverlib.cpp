#include "server.hpp"
#include <chrono>
#include <iostream>

using boost::asio::ip::tcp;

// Borrowings from lessons
template <typename Class, typename Function>
auto delegate(std::shared_ptr<Class> ptr, Function fun) {
    return [ ptr, fun ]<typename... Args>(Args && ... arg) {
        return (ptr.get()->*fun)(std::forward<Args>(arg)...);
    };
}

session::session(boost::asio::io_context &io_context, tcp::socket t_socket)
    : socket(std::move(t_socket)), timer(io_context),
      strand(io_context.get_executor()) {}
// End of borrowings from lessons

void session::go(
    std::string ID) {
    auto self(shared_from_this());
    boost::asio::spawn(strand, [this, self,
                                ID](boost::asio::yield_context yield) {
        try {
            for (;;) {
                /// Keeps the data of the message that was received from Clients
                /// 
                std::string data;
                timer.expires_from_now(std::chrono::minutes(10));
                boost::asio::async_read_until(
                    socket, boost::asio::dynamic_buffer(data), "\n", yield);
                for (auto reader : readers) {
                    if (ID != reader.second) {
                        boost::asio::async_write(
                            reader.first->socket,
                            boost::asio::buffer(ID + ": " + data), yield);
                    } else {
                        boost::asio::async_write(
                            reader.first->socket,
                            boost::asio::buffer("You: " + data), yield);
                    }
                }
                // https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/overview/core/line_based.html
            }
        } catch (std::exception &e) {
            socket.close();
            timer.cancel();
            std::cerr << "Exception: " << e.what() << "\n";
        }
    });

    boost::asio::spawn(strand, delegate(self, &session::timer_callback));
}

void session::timer_callback(boost::asio::yield_context yield) {
    while (socket.is_open()) {

        /// Describes an object used to hold error code values
        boost::system::error_code ignored_ec;
        timer.async_wait(yield[ignored_ec]);
        if (timer.expires_from_now() <= std::chrono::seconds(0))
            socket.close();
    }
}

/// Array of information about Clients
std::vector<std::pair<std::shared_ptr<session>, std::string>> session::clients;

/// Array of information about Readers
std::vector<std::pair<std::shared_ptr<session>, std::string>> session::readers;