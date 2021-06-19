#pragma once
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <memory>
#include <vector>

/* Objects of this class are individual participants in the network interaction
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

    /* Ensures the work of a Client */
    void read();

    /// Array of all Users
    static std::vector<std::shared_ptr<session>> users;

  private:
    /// Provides core I/O functionality
    boost::asio::ip::tcp::socket socket;

    /// Provides serialised function invocation for any executor type
    boost::asio::strand<boost::asio::io_context::executor_type> strand;
};