#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

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

#define UDP_BROADCAST_ADDRESS_IPV4 "255.255.255.255"

namespace biglog
{

class basic_udp_client : public enable_shared_from_this<basic_udp_client>,
    private boost::noncopyable
{
public:

        basic_udp_client(boost::asio::io_service& ios, const std::string& server, short port);

        virtual size_t write(const char* s, size_t nbytes);
        virtual size_t write(const std::string& s);

        void async_read();

        virtual void on_data(const char* data, size_t bytes_recvd) {}
        virtual void on_data_2(const char* data, size_t bytes_recvd) {}

        size_t get_max_frame_size() const { return (superjumbo_?1024*63:jumbo_?1024*8:1400); }
        std::string get_server() const { return server_; }
        short get_port() const { return port_; }

        void set_jumbo() { jumbo_ = true; }
        void set_superjumbo() { superjumbo_ = true; }

        boost::asio::ip::udp::socket& get_socket() { return socket_; }

private:

        void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);

        boost::asio::io_service& ios_;
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::resolver resolver_;
        boost::asio::ip::udp::resolver::iterator iterator_;
        boost::asio::ip::udp::endpoint sender_endpoint_;
        boost::asio::strand* strand_;

        std::string server_;
        short port_;
        bool reliable_, jumbo_, superjumbo_;
        enum { max_length = (1024*64)-1 };
        char data_[max_length+1];

        friend class reliable_udp_client;
};

};

#endif

