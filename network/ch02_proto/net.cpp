/* 
 * File:   net.cpp
 * Author: ycherkasov
 * 
 * Created on 15 Январь 2011 г., 14:57
 */
#include <sstream>
#include "common.h"
#include "net.h"
#include "server_handlers.h"

using std::cout;
using std::endl;
using std::string;
using std::runtime_error;
using std::ostringstream;


namespace{


}

proto_map_t net_socket::_protocols;

net_socket::net_socket(const string& proto)
    : _sock()
    , _proto(proto) {

    bzero(&_local, sizeof (_local));

    // socket types
    _protocols["tcp"] = SOCK_STREAM;
    _protocols["udp"] = SOCK_DGRAM;
    _protocols["raw"] = SOCK_RAW;
    _protocols["t"] = SOCK_STREAM;
    _protocols["u"] = SOCK_DGRAM;
    _protocols["r"] = SOCK_RAW;
}

net_socket::~net_socket() {
    ::close(_sock);
}

int net_socket::get_proto_value(const char* s){
    if(s == 0)
	throw std::runtime_error("unknown mode");
    proto_map_t::iterator it = _protocols.begin();
    if ((it = _protocols.find(s)) != _protocols.end())
	return (*it).second;
    else
	throw std::runtime_error("unknown mode");
}

void net_socket::init(const std::string& addr, const std::string& srv){
    _addr = addr;
    _service = srv;
    _socket_type = get_proto_value(_proto.c_str());
    
    // transform string parameters to server data
    resolve();

    // open socket
    _sock = ::socket(AF_INET, _socket_type, 0);
    if (!isvalidsock(_sock))
	throw ("socket call failed");
}

void net_socket::accept(SOCKET s){

    // accept socket
    socklen_t peerlen = sizeof (_local);
    _sock = ::accept(s, (struct sockaddr*)&_local, &peerlen);
    if (!isvalidsock(_sock))
	throw ("socket call failed");
}


void net_socket::resolve(){

    char *endptr = 0;
    short port;

    // set inet protocol
    _local.sin_family = AF_INET;

    // resolve server address or hostname
    if ( !_addr.empty() ) {
	// if it isn't IP-address, resolve name
	if (!inet_aton(_addr.c_str(), &(_local.sin_addr) )) {
	    hostent* hp = gethostbyname( _addr.c_str() );
	    if (hp == NULL){
		ostringstream err;
		err << "unknown host:" << _addr << endl;
		throw runtime_error(err.str().c_str());
	    }
	    _local.sin_addr = *(struct in_addr *)hp->h_addr;
	}
    }
    // or listen all interfaces
    else{
	_local.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    // set port number from string number
    port = strtol(_service.c_str(), &endptr, 0);
    if (*endptr == '\0'){
	_local.sin_port = htons(port);
    }
    // or from service name
    else {
	servent* sp = getservbyname( _service.c_str() ,  _proto.c_str() );
	if (sp == NULL) {
	    ostringstream err;
	    err << "unknown service:" << _service << endl;
	    throw runtime_error(err.str().c_str());
	}
	// if unable, use port order
	_local.sin_port = sp->s_port;
    }

}

//---------------------------------------------------
void tcp_proto::listen(){

	static const int on = 1;
        // set option "reuse address"
        if (setsockopt(net().socket(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on))) {
            throw runtime_error("setsockopt failed");
        }
        cout << "Setting socket options" << endl;


        // bind socket to address:port
        if (bind(net().socket(), net().sockaddr(), net().socklen())) {
            throw runtime_error("bind failed");
        }
        cout << "Bind to socket " << net().socket() << " at host " << net().address() << endl;

        // start listening
        if (::listen(net().socket(), NLISTEN)) {
            throw runtime_error("listen failed");
        }
        cout << "Started listening at port " << net().port() << endl;

        // accept incoming messages in loop
        do {
            //socklen_t peerlen = sizeof (_peer);
            net_socket peer(net().protocol());
            peer.accept(net().socket());
            if (!isvalidsock(peer.socket())) {
                throw runtime_error("client accept failed");
            }
            cout << "Client accepted " << peer.address()
                    << " at port " << peer.port() << endl;

            // custom
	    if(handler())
		handler()->server_callback(peer.socket());
        }
        while (1);
}

void tcp_proto::connect(){

    // connect client socket to server
    if (::connect(net().socket(), net().sockaddr(), net().socklen() ) ){
	throw runtime_error("connect failed");
    }
    if(handler())
	handler()->client_callback(net().socket());

}

void udp_proto::listen() {

    // udp listening more simple
    if (bind(net().socket(), net().sockaddr(), net().socklen()))
	throw runtime_error("bind failed");
    // custom
    //handler();
}

void udp_proto::connect() {}