#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <memory>
#include "Router.hpp"

namespace Core {
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;

using tcp = asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
 public:
    Session(tcp::socket socket, Router& router);

    ~Session();

    void Run();

 private:
    tcp::socket socket;
    beast::flat_buffer buffer;
    http::request<http::string_body> request;
    Router& router;

    asio::steady_timer readTimer;
    asio::steady_timer writeTimer;

    void Read();
    void Write(http::response<http::string_body> response);
};
}  // namespace Core
