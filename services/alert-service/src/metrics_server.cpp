#include <boost/asio.hpp>
#include <thread>
#include "metrics.hpp"
#include <iostream>

using boost::asio::ip::tcp;

void startMetricsServer(unsigned short port) {
    std::thread([port]() {
        try {
            boost::asio::io_context io;
            tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), port));

            while (true) {
                tcp::socket socket(io);
                acceptor.accept(socket);

                std::string body = 
                    "alert_triggered_total " + std::to_string(alert_triggered_total.load()) + "\n" +
                    "anomaly_detected_total " + std::to_string(anomaly_detected_total.load()) + "\n";

                std::string response =
                    "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " +
                    std::to_string(body.size()) + "\r\n\r\n" + body;

                boost::asio::write(socket, boost::asio::buffer(response));
            }
        } catch (const std::exception& e) {
            std::cerr << "Metrics server error: " << e.what() << std::endl;
        }
    }).detach();
}
