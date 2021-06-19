#include "reader.hpp"
#include <charconv>
#include <iostream>
#include <sstream>

using boost::asio::ip::tcp;

int main() {
    try {
        /// Provides core I/O functionality
        boost::asio::io_context io_context;

        /// Describes an object used to hold error code values
        boost::system::error_code ec;

        /// The TCP socket type
        tcp::socket socket(io_context);

        /// IPv4-address to connect to
        std::string addr = "127.0.0.1";

        /// Port to connect to
        unsigned short port = 1234;

        // Uncomment this to enter IPv4 and Port by the user
        /*std::string addr;
         unsigned short port;
         std::cout << "Enter IP-address and port:" << std::endl;
         std::cin >> addr >> port;*/

        /// An argument placeholder, for use with boost::bind(), that
        /// corresponds to the results argument of a handler for asynchronous
        /// functions such as boost::asio::async_connect
        boost::asio::ip::tcp::endpoint serv_addr(
            boost::asio::ip::make_address(addr, ec), port);
        socket.connect(serv_addr, ec);

        if (!ec) {
            socket.wait(socket.wait_read);

            /// Determine the number of bytes available for reading
            size_t bytes = socket.available();
            if (bytes > 0) {

                /// Keeps the data of the message that was received from the
                /// server
                std::vector<char> data(bytes);
                socket.read_some(boost::asio::buffer(data.data(), data.size()),
                                 ec);
                for (auto c : data) {
                    std::cout << c;
                }
            }
            std::make_shared<session>(io_context, std::move(socket))->read();
        } else {
            std::cerr << ec << "\n";
        }

        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    system("pause");
    return 0;
}