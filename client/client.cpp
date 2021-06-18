#include "client.hpp"
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <charconv>
#include <iostream>
#include <sstream>

using boost::asio::ip::tcp;

//заимствование с семинара
template <typename Class, typename Function>
auto delegate(std::shared_ptr<Class> ptr, Function fun) {
    return [ ptr, fun ]<typename... Args>(Args && ... arg) {
        return (ptr.get()->*fun)(std::forward<Args>(arg)...);
    };
}

session::session(boost::asio::io_context &io_context, tcp::socket t_socket)
    : socket(std::move(t_socket)), strand(io_context.get_executor()) {}
//конец заимствовани€

/** ‘ункци€ внутри класса session, котора€ посто€нно провер€ет 
        вход€щие сообщени€ и выводит на экран при получении*/

void session::write() {
    auto self(shared_from_this());
    boost::asio::spawn(strand, [this, self](boost::asio::yield_context yield) {
        try {
            pairs own_keys = psedo_rsa_keys();
            open_key = own_keys.open_key;
            std::pair<int, int> own_k_private = own_keys.private_key;

            if (users.size() == 1) {
                pairs chat_keys = psedo_rsa_keys();
                std::pair<int, int> own_k_open = chat_keys.open_key;
                std::pair<int, int> own_k_private = chat_keys.private_key;
                int chat_e, chat_n, chat_d;
                chat_e = own_k_open.first;
                chat_d = own_k_private.first; 
                chat_n = own_k_open.second;
            } else {
                std::vector<int> data;
                data = encryption("Hi I`m new user. Please give me keys",
                                  users[0]->open_key.first,
                                  users[0]->open_key.second)
                    std::string request = parse(data); 
                boost::asio::async_write(
                    users[0] -> socket, boost::asio::buffer(data + "\n"), yield);
            }

            std::string ID;
            for (;;) {
                std::cout << "Enter your name: ";
                std::getline(std::cin, ID); //запрет на им€ меньше 3 символов, при вводе имени 0, 1, 2 символа клиент будет запрашивать им€ повторно, пока не получит разрешЄнное
                if (ID.size() < 3) {
                    std::cout << "Incorrect name. You should use longer name\n";
                } else {
                    std::cout << "Good job! Join reader.exe now\n";
                    break;
                }
            }
            boost::asio::async_write(
                socket, boost::asio::buffer("c" + ID + "\n"), yield); //c - нужно, чтобы сервер мог пон€ть, client это или reader.

            for (;;) { //в этом цикле async_write и async_read наход€тс€ в специальном пор€дке, в каком этого требует сервер.
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
                if (data == "Connected to Reader\n") { //при удачном соединении с reader`ом, сервер присылает данную строку. ≈сли прислал другую - значит произошла ошибка. » будет запущено присоединение к reader`у повторно.
                    std::cout << data;
                    break;
                }
            }

            for (;;) { // основной цикл отправки сообщений
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

std::vector<std::shared_ptr<session>> session::users;


/** ¬ функции происходит соединение к серверу*/
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
            socket.wait(socket.wait_read); //ожидание и чтение об€зательного сообщени€ с сервера.
            size_t bytes = socket.available();
            if (bytes > 0) {
                std::vector<char> data(bytes);
                socket.read_some(boost::asio::buffer(data.data(), data.size()),
                                 ec);
                for (auto c : data) {
                    std::cout << c;
                }
            }
            std::make_shared<session>(io_context, std::move(socket))->write();
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