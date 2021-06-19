#pragma once
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <memory>
#include <vector>

/* Object of this class is Server
 */
class session : public std::enable_shared_from_this<session> {

  public:
    /**
     *   Constructor of class session
     *   \param io_context Provides core I/O functionality.
     *   \param t_socket The TCP socket type
     */
    explicit session(boost::asio::io_context &io_context,
                     boost::asio::ip::tcp::socket t_socket);

    /**
    *    Ensures the work of a Server
    *    \param ID Name of User
    */
    void go(std::string ID);

    /*
    *    Countdown time
    *    \param yield_cotext Context object that represents the currently executing coroutine
    */
    void timer_callback(boost::asio::yield_context);

    /// Array of information about Clients
    static std::vector<std::pair<std::shared_ptr<session>, std::string>>
        clients;

    /// Array of information about Readers 
    static std::vector<std::pair<std::shared_ptr<session>, std::string>>
        readers;

    /// Provides core I/O functionality
    boost::asio::ip::tcp::socket socket;

    /// Shows whether a pair has been created
    bool pair;

  private:

    /// Typedef for a timer based on the steady clock
    boost::asio::steady_timer timer;

    /// Provides serialised function invocation for any executor type
    boost::asio::strand<boost::asio::io_context::executor_type> strand;
};