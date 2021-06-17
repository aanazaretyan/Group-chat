#include "catch.hpp"
#include "server.hpp"
#include <sstream>
#include <charconv>
#include <iostream>



/*TEST_CASE("Own message") {
    boost::system::error_code ec;
    boost::asio::io_context io_context_serv;
    boost::asio::ip::tcp::socket socket_serv(io_context_serv);
    

    auto addr =
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234);
    boost::asio::spawn(io_context_serv, [&](boost::asio::yield_context yield) {
        boost::asio::ip::tcp::acceptor acceptor(io_context_serv, addr);
        
        std::string ID = "User";
        auto address =
            std::make_shared<session>(io_context_serv, std::move(socket_serv));
        address->go(ID);
        


        boost::asio::io_context io_context_cli;
        boost::asio::ip::tcp::socket socket_cli(io_context_cli);
        socket_cli.connect(addr, ec);
        std::string data = "Hello!";
        socket_cli.write_some(
            boost::asio::buffer(data.data(), data.size()), ec);
        std::string responce;
        boost::asio::async_read_until(socket_cli,
                                      boost::asio::dynamic_buffer(responce),
                                      "\n", yield);
        std::string check = ID + ": " + data;
        

        REQUIRE(responce == check);
    });
}*/