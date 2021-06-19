#include "reader.hpp"
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

void session::read() {
    
    // shared_ptr of class
    auto self(shared_from_this());
    boost::asio::spawn(strand, [this, self](boost::asio::yield_context yield) {
        try {

            /// Keeps the name of the User
            std::string ID;
            std::cout << "Enter your name: ";
            std::getline(std::cin, ID);
            boost::asio::async_write(
                socket, boost::asio::buffer("r" + ID + "\n"), yield);
            for (;;) {

                /// Keeps the data of the message that was received from the
                /// server
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
/// Array of all Users
std::vector<std::shared_ptr<session>> session::users;