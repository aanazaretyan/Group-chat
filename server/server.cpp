#include "server.hpp"
#include <chrono>
#include <iostream>

using boost::asio::ip::tcp;

//заимствование с семинара
template <typename Class, typename Function>
auto delegate(std::shared_ptr<Class> ptr, Function fun) {
    return [ ptr, fun ]<typename... Args>(Args && ... arg) {
        return (ptr.get()->*fun)(std::forward<Args>(arg)...);
    };
}

session::session(boost::asio::io_context &io_context, tcp::socket t_socket)
    : socket(std::move(t_socket)), timer(io_context),
      strand(io_context.get_executor()) {}
//конец взаимствования 

/** Функция класса session, которая все полученые  на сервер сообщения передаёт всем клиентам */
void session::go(std::string ID) {
    auto self(shared_from_this());
    boost::asio::spawn(
        strand, [this, self, ID](boost::asio::yield_context yield) {
            try {
                for (;;) {
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
                            boost::asio::async_write( // если сервер понимает, что клиент, которому сейчас должен отправить сообщение является автором, то вместо имени автора пишет "You".
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
        boost::system::error_code ignored_ec;
        timer.async_wait(yield[ignored_ec]);
        if (timer.expires_from_now() <= std::chrono::seconds(0))
            socket.close();
    }
}

std::vector<std::pair<std::shared_ptr<session>, std::string>> session::clients;
std::vector<std::pair<std::shared_ptr<session>, std::string>> session::readers;

//** Функция создаёт пару client-reader*/
int main() {
    try {
        unsigned short port = 1234;
        boost::asio::io_context io_context;

        boost::asio::spawn(io_context, [&](boost::asio::yield_context yield) {
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

            while (true) {
                boost::system::error_code ec;
                tcp::socket socket(io_context);
                acceptor.async_accept(socket, yield[ec]);

                for (auto client : session::clients) { //цикл удаляет указатель из массива, если сокет клиента уже закрыт
                    if (!(client.first->socket.is_open())) {
                        session::clients.erase(
                            std::find(session::clients.begin(),
                                      session::clients.end(), client));
                    }
                }
                for (auto reader : session::readers) { //цикл удаляет указатель из массива, если сокет reader`а уже закрыт
                    if (!(reader.first->socket.is_open())) {
                        session::readers.erase(
                            std::find(session::readers.begin(),
                                      session::readers.end(), reader));
                    }
                }
                if (!ec) {
                    std::cout << "New connection" << std::endl;
                    std::string message = "Hi, you are connected!\n";
                    socket.write_some(
                        boost::asio::buffer(message.data(), message.size()),
                        ec);
                    std::string ID;
                    boost::asio::async_read_until(
                        socket, boost::asio::dynamic_buffer(ID), "\n", yield);
                    ID.pop_back();
                    if (ID[0] == 'c') {//первым сообщением клиент или reader отправляют опознавательный знак, чтобы сервер их различал
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
                        auto address = std::make_shared<session>(
                            io_context, std::move(socket));
                        std::pair<std::shared_ptr<session>, std::string> sess =
                            {address, ID};
                        session::clients.push_back(sess);
                    } else {
                        ID.erase(0, 1);
                        for (auto client : session::clients) { //много условий, из-за которых сервер закроет соединение
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
                                std::string responce;
                                boost::asio::async_read_until(
                                    client.first->socket,
                                    boost::asio::dynamic_buffer(responce), "\n",
                                    yield);
                                client.first->go(ID);
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