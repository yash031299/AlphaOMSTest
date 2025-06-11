#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include "health_check.hpp"

using boost::asio::ip::tcp;

void startHealthCheckServer(unsigned short port) {
    std::thread([](unsigned short p) {
        try {
            boost::asio::io_context io;
            tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), p));

            while (true) {
                tcp::socket socket(io);
                acceptor.accept(socket);

                std::string response =
                    "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nALERT-SERVICE OK";

                boost::system::error_code ignored_error;
                boost::asio::write(socket, boost::asio::buffer(response), ignored_error);
            }
        } catch (const std::exception& e) {
            std::cerr << "Healthcheck server error: " << e.what() << std::endl;
        }
    }, port).detach();
}
