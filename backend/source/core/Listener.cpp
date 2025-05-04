#include "Listener.hpp"

#include <boost/asio/strand.hpp>
#include <iostream>
#include "Session.hpp"

using namespace Core;

Listener::Listener(asio::io_context& ioContext, tcp::endpoint endpoint) : acceptor(asio::make_strand(ioContext)) {
    beast::error_code errorCode;

    this->acceptor.open(endpoint.protocol(), errorCode);

    if (errorCode) {
        std::cerr << "Open error: " << errorCode.message() << '\n';
        return;
    }

    this->acceptor.set_option(asio::socket_base::reuse_address(true), errorCode);

    if (errorCode) {
        std::cerr << "Set option error: " << errorCode.message() << '\n';
        return;
    }

    this->acceptor.bind(endpoint, errorCode);

    if (errorCode) {
        std::cerr << "Bind error: " << errorCode.message() << '\n';
        return;
    }

    this->acceptor.listen(asio::socket_base::max_listen_connections, errorCode);

    if (errorCode) {
        std::cerr << "Listen error: " << errorCode.message() << '\n';
        return;
    }

    std::cout << "Server started.\n";

    Accept(ioContext);
}

Listener::~Listener() = default;

void Listener::Accept(asio::io_context& ioContext) {
    this->acceptor.async_accept(asio::make_strand(ioContext), [this, &ioContext](beast::error_code errorCode, tcp::socket socket) {
        if (!errorCode) {
            std::shared_ptr<Session> session = std::make_shared<Session>(std::move(socket), this->router);

            session->Run();
        }

        this->Accept(ioContext);
    });
}
