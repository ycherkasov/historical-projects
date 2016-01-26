/* 
 * File:   net.h
 * Author: ycherkasov
 *
 * Created on 15 Январь 2011 г., 14:57
 */

#ifndef NET_H
#define	NET_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include "app.h"
#include "server_handlers.h"

/* UNIX version */

typedef std::map<std::string, int> proto_map_t;

/**@brief Network socket wrapper class */
class net_socket {
public:

    /**@brief Network socket for existing protocol */
    net_socket(const std::string& proto);

    /**@brief Cleanup (close socket) */
    virtual ~net_socket();
    
private:
    net_socket(const net_socket& orig);
    net_socket& operator=(const net_socket& orig);
    

protected:

    /**@brief Resolve address, service name etc. */
    void resolve();

public:

    /**@brief Create socket */
    void init(const std::string& addr, const std::string& srv);

    /**@brief Create socket to accept connection */
    void accept(SOCKET s);

    /**@brief Map string value to protocol enum */
    int get_proto_value(const char* s);

    /**@brief Native socket */
    const SOCKET socket() const {return _sock;}

    /**@brief Sockaddr structure */
    struct sockaddr* sockaddr(){return reinterpret_cast<struct sockaddr*>(&_local);}

    /**@brief Sockaddr structure lenght */
    socklen_t socklen() const {return sizeof(_local);}

    /**@brief IP-address string representation */
    std::string address() const {return inet_ntoa(_local.sin_addr);}

    /**@brief Port string representation */
    uint16_t port() const {return ntohs(_local.sin_port);}

    /**@brief Protocol string representation */
    std::string protocol() const {return _proto;}


protected:

    /**@brief Map string value to protocol enum */
    static proto_map_t _protocols;

    /**@brief Sockaddr structure (address, port) */
    sockaddr_in _local; 

    /**@brief Address string representation (initial) */
    std::string _addr;

    /**@brief Service string representation (initial) */
    std::string _service;

    /**@brief Protocol string representation (initial) */
    std::string _proto;

    /**@brief Socket type (TCP/UDP/RAW) */
    int _socket_type;

    /**@brief Socket id */
    SOCKET _sock;
};

////////////////////////////////////////////////////
#ifdef UDP
for (;;) {

    peerlen = sizeof ( peer);

    if (recvfrom(s, buf, sizeof ( buf), 0, (struct sockaddr*) &peer, &peerlen) < 0)
        throw runtime_error("error recvfrom");

    if (sendto(s, "hello, world\n", 13, 0, (struct sockaddr*) &peer, peerlen) < 0)
        error(1, errno, "error sendto");

}
#endif

class iproto {
public:
    virtual void init(const std::string& addr, const std::string& srv) = 0;
    virtual void listen() = 0;
    virtual void stop_listen() = 0;
    virtual void connect() = 0;
};

class proto : public iproto{
    //typedef void (*net_handler_t)(SOCKET, char*, size_t);
public:
    proto(const std::string& p, i_packets_handler* h)
        : _net(p), _packet_handler(h){}
    virtual void init(const std::string& addr, const std::string& srv){
        _net.init(addr, srv);
    }
protected:
    net_socket& net(){return _net;}
    i_packets_handler* handler(){return _packet_handler;}
private:
    net_socket _net;
    i_packets_handler* _packet_handler;
};


class tcp_proto : public proto {
public:

    tcp_proto(i_packets_handler* hndl) : proto("tcp", hndl){}

    virtual void init(const std::string& addr, const std::string& srv) {
        proto::init(addr, srv);
    }

    virtual void listen();
    virtual void stop_listen(){
        throw std::runtime_error("tcp_proto::stop_listen not implemented");
    };
    virtual void connect();
};

class udp_proto : public proto {
public:

    udp_proto(i_packets_handler* hndl): proto("udp", hndl){}

    virtual void init(const std::string& addr, const std::string& srv) {
        proto::init(addr, srv);
    }

    virtual void listen();
    virtual void stop_listen(){
        throw std::runtime_error("udp_proto::stop_listen not implemented");
    };

    virtual void connect();
};

////////////////////////////////////////////////
class server {
    proto* _proto_impl;
public:
    server(proto* proto_impl):_proto_impl(proto_impl){}
    void listen(const std::string& addr, const std::string& srv) {
        _proto_impl->init(addr, srv);
        _proto_impl->listen();
    }
};

class client {
    proto* _proto_impl;
public:
    client(proto* proto_impl):_proto_impl(proto_impl){}
    void connect(const std::string& addr, const std::string& srv) {
        _proto_impl->init(addr, srv);
        _proto_impl->connect();
    }
};



#endif	/* NET_H */

