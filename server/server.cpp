#include "server.hpp"
#include <chrono>
#include <iostream>

using boost::asio::ip::tcp;


int main() {
    try {

        ///Port to accept to
        unsigned short port = 1234;

        /// Provides core I/O functionality
        boost::asio::io_context io_context;

        boost::asio::spawn(io_context, [&](boost::asio::yield_context yield) {
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

            while (true) {

                /// Describes an object used to hold error code values
                boost::system::error_code ec;

                /// The TCP socket type
                tcp::socket socket(io_context);
                acceptor.async_accept(socket, yield[ec]);

                for (auto client : session::clients) {
                    if (!(client.first->socket.is_open())) {
                        session::clients.erase(
                            std::find(session::clients.begin(),
                                      session::clients.end(), client));
                    }
                }
                for (auto reader : session::readers) {
                    if (!(reader.first->socket.is_open())) {
                        session::readers.erase(
                            std::find(session::readers.begin(),
                                      session::readers.end(), reader));
                    }
                }
                if (!ec) {
                    std::cout << "New connection" << std::endl;
                    
                    /// Keeps the data of the messages to and from Users
                    std::string message;
                    message = "Hi, you are connected!\n";
                    socket.write_some(
                        boost::asio::buffer(message.data(), message.size()),
                        ec);

                    /// Keeps the name of the User
                    std::string ID;
                    boost::asio::async_read_until(
                        socket, boost::asio::dynamic_buffer(ID), "\n", yield);
                    ID.pop_back();
                    if (ID[0] == 'c') {
                        ID.erase(0, 1);
                        for (auto client : session::clients) {
                            if (client.second == ID) {
                                message = "Network already has a user with "
                                          "this name\n";
                                socket.write_some(
                                    boost::asio::buffer(message.data(),
                                                        message.size()),
                                    ec);
                                socket.close();
                            }
                        }

                        /// Address of session of Client
                        auto address = std::make_shared<session>(
                            io_context, std::move(socket));

                        ///Information about session of Client
                        std::pair<std::shared_ptr<session>, std::string> sess =
                            {address, ID};
                        session::clients.push_back(sess);
                    } else {
                        ID.erase(0, 1);
                        for (auto client : session::clients) {
                            if (client.second == ID) {
                                for (auto reader : session::readers) {
                                    if (reader.second == ID) {
                                        message = "Network already has a "
                                                  "reader with this name\n";
                                        socket.write_some(
                                            boost::asio::buffer(message.data(),
                                                                message.size()),
                                            ec);
                                        socket.close();
                                    }
                                }
                                message = "Accept the request on client.exe, "
                                          "where you were logged in as " +
                                          ID + ". Write (Y)es/(N)ot\n";
                                socket.write_some(
                                    boost::asio::buffer(message.data(),
                                                        message.size()),
                                    ec);
                                message = "Accept the request here " + ID +
                                          ". Write (Y)es/(N)ot\n";
                                client.first->socket.write_some(
                                    boost::asio::buffer(message.data(),
                                                        message.size()),
                                    ec);

                                /// Keeps data of responce from Client
                                std::string responce;
                                boost::asio::async_read_until(
                                    client.first->socket,
                                    boost::asio::dynamic_buffer(responce), "\n",
                                    yield);
                                client.first->go(ID);

                                /// First symbol of responce from Client
                                char res = responce[0];
                                res = std::tolower(res);
                                if (res == 'y') {
                                    auto address = std::make_shared<session>(
                                        io_context, std::move(socket));
                                    address->pair = true;
                                    message = "Connected to Reader\n";
                                    client.first->socket.write_some(
                                        boost::asio::buffer(message.data(),
                                                            message.size()),
                                        ec);
                                    std::pair<std::shared_ptr<session>,
                                              std::string>
                                        sess = {address, ID};
                                    session::readers.push_back(sess);
                                    address->go(ID);
                                } else {
                                    message = "Your request was rejected\n";
                                    client.first->socket.write_some(
                                        boost::asio::buffer(message.data(),
                                                            message.size()),
                                        ec);
                                    socket.close();
                                }
                            }
                        }
                        if (!(std::make_shared<session>(io_context,
                                                        std::move(socket))
                                  ->pair)) {
                            message = "User with this name not found\n";
                            socket.write_some(
                                boost::asio::buffer(message.data(),
                                                    message.size()),
                                ec);
                            socket.close();
                        }
                    }
                } else {
                    std::cerr << ec << "\n";
                }
            }
        });

        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
        system("pause");
    }
}