#include "client.hpp"
#include <charconv>
#include <iostream>
#include <sstream>

using boost::asio::ip::tcp;

// Borrowings from lessons
template <typename Class, typename Function>
auto delegate(std::shared_ptr<Class> ptr, Function fun) {
    return [ ptr, fun ]<typename... Args>(Args && ... arg) {
        return (ptr.get()->*fun)(std::forward<Args>(arg)...);
    };
}

session::session(boost::asio::io_context &io_context, tcp::socket t_socket)
    : socket(std::move(t_socket)), strand(io_context.get_executor()) {}
// End of borrowings from lessons

void session::write() {

    ///shared_ptr of class
    auto self(shared_from_this());
    boost::asio::spawn(strand, [this, self](boost::asio::yield_context yield) {
        try {

            /// Keeps the name of the User
            std::string ID;
            for (;;) {
                std::cout << "Enter your name: ";
                std::getline(std::cin, ID);
                if (ID.size() < 3) {
                    std::cout << "Incorrect name. You should use longer name\n";
                } else {
                    std::cout << "Good job! Join reader.exe now\n";
                    break;
                }
            }
            boost::asio::async_write(
                socket, boost::asio::buffer("c" + ID + "\n"), yield);

            for (;;) {

                /// Keeps the data of the message that was received from the
                /// server
                std::string data;
                boost::asio::async_read_until(
                    socket, boost::asio::dynamic_buffer(data), "\n", yield);
                std::cout << data;
                std::getline(std::cin, data);
                boost::asio::async_write(
                    socket, boost::asio::buffer(data + "\n"), yield);
                data = "";
                boost::asio::async_read_until(
                    socket, boost::asio::dynamic_buffer(data), "\n", yield);
                if (data == "Connected to Reader\n") {
                    std::cout << data;
                    break;
                }
            }

            for (;;) {

                /// Keeps the data of the message that was received from the
                /// keyboard of User
                std::string body;
                std::getline(std::cin, body);
                boost::asio::async_write(
                    socket, boost::asio::buffer(body + "\n"), yield);
            }
        } catch (std::exception &e) {
            socket.close();
            std::cerr << "Exception: " << e.what() << "\n";
        }
    });
}

/// Array of all Users
std::vector<std::shared_ptr<session>> session::users;