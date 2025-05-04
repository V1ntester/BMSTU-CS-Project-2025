#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include "Router.hpp"

namespace Core {
namespace asio = boost::asio;

using tcp = asio::ip::tcp;

class Listener : public std::enable_shared_from_this<Listener> {
 public:
    Listener(asio::io_context& ioContext, tcp::endpoint endpoint);

    ~Listener();

 private:
    tcp::acceptor acceptor;
    Router router;

    void Accept(asio::io_context& ioContext);
};
}  // namespace Core
