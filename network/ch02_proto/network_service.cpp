/* 
 * File:   network_service.cpp
 * Author: ycherkasov
 * 
 * Created on 18 Январь 2011 г., 17:32
 */

#include <string>
#include <stdexcept>
#include "network_service.h"
#include "server_handlers.h"
#include "app.h"

network_service::network_service(const std::string& proto
        , const std::string& addr, const std::string& srv
	, const std::string& hndl) 
    : _addr(addr)
    , _service(srv)
    , _cln(), _srv()
    , _proto_impl( network_factory(proto) )
    , _handler( hndl ){}

network_service::~network_service() {
    delete _srv;
    delete _cln;
    delete _proto_impl;
}

void network_service::start(){
    _srv = new server(_proto_impl);
    _srv->listen(_addr, _service);
}

void network_service::connect(){
    _cln = new client(_proto_impl);
    _cln->connect(_addr, _service);
}

proto* network_service::network_factory(const std::string& pr){

    if(pr.compare("tcp") == 0){
	return new tcp_proto( handler_factory(_handler) );
    }
    else if(pr.compare("udp") == 0){
	return new udp_proto( handler_factory(_handler) );
    }
    else{
	throw std::runtime_error("Unknown protocol");
    }
    return 0;
}

i_packets_handler* network_service::handler_factory(const std::string& h){

    if(h.compare("fixed") == 0){
	return new fixed_lenght_buffer();
    }
    else if(h.compare("safe-fixed") == 0){
	return new safe_buffer();
    }
    else if(h.compare("variable") == 0){
	return new variable_lenght_message();
    }
    else{
	throw std::runtime_error("Unknown protocol");
    }
    return 0;
}

