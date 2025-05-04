#include "Session.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include "Router.hpp"

using namespace Core;

namespace {
const size_t kReadTimeout = 10000;
const size_t kWriteTimeout = 3000;
}  // namespace

Session::Session(tcp::socket socket, Router& router)
    : socket(std::move(socket)), router(router), readTimer(this->socket.get_executor()), writeTimer(this->socket.get_executor()) {
}

Session::~Session() = default;

void Session::Run() {
    this->Read();
}

void Session::Read() {
    std::shared_ptr<Session> self = shared_from_this();

    readTimer.expires_after(std::chrono::milliseconds(kReadTimeout));
    readTimer.async_wait([self](boost::system::error_code errorCode) {
        if (!errorCode) {
            self->socket.close();
        }
    });

    http::async_read(this->socket, this->buffer, this->request, [self](beast::error_code errorCode, size_t) {
        self->readTimer.cancel();

        if (!errorCode) {
            http::response<http::string_body> response = self->router.Route(self->request);

            self->Write(response);
        }
    });
}

void Session::Write(http::response<http::string_body> response) {
    std::shared_ptr<Session> self = shared_from_this();

    writeTimer.expires_after(std::chrono::milliseconds(kWriteTimeout));
    writeTimer.async_wait([self](boost::system::error_code errorCode) {
        if (!errorCode) {
            self->socket.close();
        }
    });

    std::shared_ptr<http::response<http::string_body>> responsePointer = std::make_shared<http::response<http::string_body>>(std::move(response));

    http::async_write(this->socket, *responsePointer, [self, responsePointer](beast::error_code errorCode, size_t) {
        self->writeTimer.cancel();
        
        self->socket.shutdown(tcp::socket::shutdown_send, errorCode);
    });
}
