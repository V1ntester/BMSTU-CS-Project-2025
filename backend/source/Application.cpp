#include "Application.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include "core/Listener.hpp"

namespace asio = boost::asio;

using tcp = boost::asio::ip::tcp;

namespace {
const unsigned short kPort = 8080;
}

Application::Application() = default;

Application::~Application() = default;

void Application::Run() {
    try {
        const asio::ip::address address = asio::ip::make_address("0.0.0.0");

        asio::io_context ioContext(1);

        std::shared_ptr<Core::Listener> listener = std::make_shared<Core::Listener>(ioContext, tcp::endpoint(address, kPort));

        ioContext.run();
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what() << '\n';
    }
}
