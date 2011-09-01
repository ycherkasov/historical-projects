#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <cfloat>
#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace boost;


class basic_udp_server : public enable_shared_from_this<basic_udp_server>,
    private boost::noncopyable
{
public:

        basic_udp_server(boost::asio::io_service& ios, const std::string& address, short port);

        std::string sender_address()
        {
                return sender_endpoint_.address().to_string();
        }

        void join_multicast_group(const std::string& multicast_address);
        long long get_fails() const { return fails_; }

        boost::asio::ip::udp::socket& get_socket() { return socket_; }

protected:

        virtual void on_data(const char* data, size_t bytes_recvd) {}
        virtual void on_data_2(char* data, size_t bytes_recvd) {}

        boost::asio::ip::udp::socket socket_;
        long long fails_;

private:

        void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);

        bool reliable_;
        std::string address_;
        boost::asio::io_service& ios_;
        boost::asio::ip::udp::endpoint sender_endpoint_;
        enum { max_length = (1024*64)-1 };
        char data_[max_length+1];

        friend class reliable_udp_server;
};

#endif

