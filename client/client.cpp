#include <iostream>
#include "lib.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <charconv>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>
#include <thread>
#include <sstream>


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

class session : public std::enable_shared_from_this<session> {
    // Curiosly recurring template pattern (CRTP)
public:
    explicit session(boost::asio::io_context& io_context, tcp::socket t_socket)
        : socket(std::move(t_socket)), //timer(io_context),
        strand(io_context.get_executor()) {
    }

    void write() {
        auto self(shared_from_this());
        boost::asio::spawn(strand, [this,
            self](boost::asio::yield_context yield) {

                try {
                    std::string message, colon;
                    std::cout << "Enter your name:\n";
                    std::getline(std::cin, message);
                    colon = ": ";
                    for (auto i : colon) {
                        message += i;
                    }
                    for (;;) {
                        std::string body;
                        std::getline(std::cin, body);
                        message += body;
                        boost::asio::async_write(
                            socket, boost::asio::buffer(message + "\n"), yield);
                    }
                }
                catch (std::exception& e) {
                    socket.close();
                }
            });
    }
    void read() {
        auto self(shared_from_this());
        boost::asio::spawn(
            strand, [this, self](boost::asio::yield_context yield) {
                try {
                    for (;;) {
                        std::string data;
                        size_t m = boost::asio::async_read_until(
                            socket, boost::asio::dynamic_buffer(data), "\n",
                            yield);
                        std::string_view text{ data };
                        std::cout << text;
                    }
                }
                catch (std::exception& e) {
                    socket.close();
                }
            });
    }

private:
    tcp::socket socket;
    boost::asio::strand<boost::asio::io_context::executor_type> strand;
};


int main(int argc, char* argv[]) {
    try {
        boost::asio::io_context io_context;
        boost::system::error_code ec;
        tcp::socket socket(io_context);
        //std::string addr = "127.0.0.1";
        //unsigned short port = 1234;
        std::string addr;
        unsigned short port;
        std::cout << "Enter IP-address and port:" << std::endl;
        std::cin >> addr >> port;
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
            std::thread th(&session::read, new_session);
            th.detach();
            new_session->write();
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