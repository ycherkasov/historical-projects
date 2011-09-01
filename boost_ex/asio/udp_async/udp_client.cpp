#include <iostream>

#include "udp_client.hpp"

using namespace biglog;

basic_udp_client::basic_udp_client(boost::asio::io_service& ios, const std::string& server, short port)
: ios_(ios), socket_(ios, boost::asio::ip::udp::endpoint(( boost::asio::ip::udp::v4()), 0)),
resolver_(ios), server_(server), port_(port) {
    strand_ = 0;
    jumbo_ = superjumbo_ = reliable_ = false;
    char sport[80];
    sprintf(sport, "%d", port);
    boost::asio::ip::udp::resolver::query query((boost::asio::ip::udp::v4()), server, sport);
    iterator_ = resolver_.resolve(query);

    if (server == UDP_BROADCAST_ADDRESS_IPV4) {
	asio::socket_base::broadcast option(true);
	socket_.set_option(option);
    }
}

void basic_udp_client::async_read() {
    if (strand_)
	socket_.async_receive_from(
	    boost::asio::buffer(data_, max_length), sender_endpoint_,
	    strand_->wrap(boost::bind(&basic_udp_client::handle_receive_from, this,
	    boost::asio::placeholders::error,
	    boost::asio::placeholders::bytes_transferred)));
    else
	socket_.async_receive_from(
	    boost::asio::buffer(data_, max_length), sender_endpoint_,
	    boost::bind(&basic_udp_client::handle_receive_from, this,
	    boost::asio::placeholders::error,
	    boost::asio::placeholders::bytes_transferred));
}

void basic_udp_client::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd) {
    if (error || (bytes_recvd < 0)) {
	return;
    }

    data_[bytes_recvd] = 0;

    if (reliable_)
	on_data_2(data_, bytes_recvd);
    else
	on_data(data_, bytes_recvd);

    async_read();
}

size_t basic_udp_client::write(const char* s, size_t nbytes) {
    try {
	return socket_.send_to(boost::asio::buffer(s, nbytes), *iterator_);
    } catch (std::exception&) {
	std::cout << "UDP CLIENT WRITE FAILED" << std::endl;
    }

    return 0;
}

size_t basic_udp_client::write(const std::string& s) {
    return write(s.data(), s.length());
}

int main(int argc, char* argv[]) {
    try {
	if (argc != 3) {
	    std::cerr << "Usage: chat_client <host> <port>\n";
	    return 1;
	}
	boost::asio::io_service io;
	basic_udp_client cli(io, "127.0.0.1", 1040);
	io.run();
    } catch (std::exception& e) {
	std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

