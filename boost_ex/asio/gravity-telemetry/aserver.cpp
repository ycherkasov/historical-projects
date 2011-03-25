#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "telemetry.h"
#include "raw_binary_archive.h"

using boost::asio::ip::udp;

enum {
    max_length = 1024
};

/** @brief Asynchronuous UDP echo-server (from boost samples) */
class udp_async_server : public boost::enable_shared_from_this<udp_async_server> {
public:

    /** @brief Start async receive on creation */
    udp_async_server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service)
    , request_()
    //, response_()
    , socket_(io_service, udp::endpoint(udp::v4(), port)) {

        outbound_data_.reserve(max_length);
        socket_.async_receive_from(
                boost::asio::buffer(&outbound_data_[0], max_length), sender_endpoint_,
                boost::bind(&udp_async_server::handle_receive_from, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    /** @brief Handle receive request from client */
    void handle_receive_from(const boost::system::error_code& error,
            size_t bytes_recvd) {

        if (!error && bytes_recvd > 0) {

            _tresp.crc8 = 0xFF;
            std::cout << "handle_receive_from::async_send_to: "
                    << _tresp.get_debug_info() << std::endl;
            
            std::ostringstream archive_stream;
            //boost::archive::binary_oarchive archive(archive_stream);
            raw_binary_oarchive archive(archive_stream);
            archive << _tresp;
            outbound_data_ = archive_stream.str();

            socket_.async_send_to(
                    boost::asio::buffer(&outbound_data_[0], TelemetryResponse::telemetryResponsePacketSize)
                    , sender_endpoint_
                    , boost::bind(&udp_async_server::handle_send_to, this
                    , boost::asio::placeholders::error
                    , boost::asio::placeholders::bytes_transferred));
        }
        else {

            // todo : handle with serialization
            socket_.async_receive_from(
                    boost::asio::buffer(request_, max_length), sender_endpoint_,
                    boost::bind(&udp_async_server::handle_receive_from, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
    }

    /** @brief Handle response to client */
    void handle_send_to(const boost::system::error_code& /*error*/,
            size_t /*bytes_sent*/) {

        std::cout << "handle_send_to::async_receive_from: "
                << request_ << std::endl;

        // as soon we send data, receive the next
        socket_.async_receive_from(
                boost::asio::buffer(request_, max_length), sender_endpoint_,
                boost::bind(&udp_async_server::handle_receive_from, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        std::memset(request_, 0x0, max_length);
    }

private:
    /** @brief Async services wrapper */
    boost::asio::io_service& io_service_;

    /** @brief UDP socket */
    udp::socket socket_;

    /** @brief Endpoint with client information */
    udp::endpoint sender_endpoint_;

    /** @brief Request/response text buffer */
    char request_[max_length];
    //char response_[max_length];
    TelemetryResponse _tresp;
    std::string outbound_data_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: async_udp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        udp_async_server s(io_service, std::atoi(argv[1]));

        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

