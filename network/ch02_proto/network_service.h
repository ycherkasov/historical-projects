/* 
 * File:   network_service.h
 * Author: ycherkasov
 *
 * Created on 18 Январь 2011 г., 17:32
 */

#ifndef NETWORK_SERVICE_H
#define	NETWORK_SERVICE_H
#include <string>
#include "net.h"
#include "server_handlers.h"

class network_service {
public:
    network_service(const std::string& proto
        , const std::string& addr
        , const std::string& srv
        , const std::string& hndl);
    virtual ~network_service();
    void start();
    void connect();
protected:
    proto* network_factory(const std::string& s);
    i_packets_handler* handler_factory(const std::string& s);
private:
    std::string _addr;
    std::string _service;
    std::string _handler;
    proto* _proto_impl;
    server* _srv;
    client* _cln;
};

#endif	/* NETWORK_SERVICE_H */

