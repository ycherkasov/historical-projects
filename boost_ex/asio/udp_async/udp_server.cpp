#include <iostream>

#include "udp_server.hpp"

basic_udp_server::basic_udp_server(boost::asio::io_service& ios, const std::string& address, short port)
: ios_(ios), socket_(ios), address_(address), reliable_(false), fails_(0) {

    std::string listen_address = (address != "*") && (address != "") ? address : ("0.0.0.0");
    boost::asio::ip::udp::endpoint listen_endpoint(boost::asio::ip::address::from_string(listen_address), port);

    socket_.open(listen_endpoint.protocol());
    socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.set_option(boost::asio::socket_base::receive_buffer_size(1024 * 64));

    socket_.bind(listen_endpoint);
    socket_.async_receive_from(
	    boost::asio::buffer(data_, max_length), sender_endpoint_,
	    boost::bind(&basic_udp_server::handle_receive_from, this,
	    boost::asio::placeholders::error,
	    boost::asio::placeholders::bytes_transferred));
}

void basic_udp_server::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd) {
    if (error || (bytes_recvd < 0)) {
	return;
    }

    data_[bytes_recvd] = 0;

    if (reliable_)
	on_data_2(data_, bytes_recvd);
    else
	on_data(data_, bytes_recvd);

    socket_.async_receive_from(
	    boost::asio::buffer(data_, max_length), sender_endpoint_,
	    boost::bind(&basic_udp_server::handle_receive_from, this,
	    boost::asio::placeholders::error,
	    boost::asio::placeholders::bytes_transferred));
}

void basic_udp_server::join_multicast_group(const std::string& multicast_address) {
    socket_.set_option(
	    boost::asio::ip::multicast::join_group(
	    boost::asio::ip::address::from_string(multicast_address)));
}

int main(int argc, char* argv[]) {
    try {
	boost::asio::io_service io;
	basic_udp_server(io, "127.0.0.1", 1040);
	io.run();

    } catch (std::exception& e) {
	std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}


