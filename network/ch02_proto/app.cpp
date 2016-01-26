#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include "app.h"
#include "net.h"
#include "network_service.h"

app::app() {
    init_maps();
}

void app::init_maps() {
    _modes["client"] = mode_client;
    _modes["cli"] = mode_client;
    _modes["server"] = mode_server;
    _modes["srv"] = mode_server;
}

void app::usage(){
    std::cout << "usage:" << std::endl;
    std::cout << "-h:\tshow help" << std::endl;
    std::cout << "-p:\tprotocol (tcp, udp, raw)" << std::endl;
    std::cout << "-m:\tmode (client, server)" << std::endl;
    std::cout << "-a:\taddress (IP, DNS)" << std::endl;
    std::cout << "-s:\tservice (port, service name)" << std::endl;
    std::cout << "-n:\tnetwork handler (fixed, safe-fixed, variable)" << std::endl;
}

app_mode_t app::get_mode_value(const char* s){

    if(s == 0)
	throw std::runtime_error("unknown mode");
    
    mode_map_t::iterator it = _modes.begin();
    if ((it = _modes.find(s)) != _modes.end())
	return (*it).second;
    else
	throw std::runtime_error("unknown mode");
}

void app::start(){

    // start application
    network_service net_srv(_params._proto
    , _params._addr
    , _params._service
    , _params._network_handler);

    if(_params._mode == mode_client){
	net_srv.connect();
    }
    else if(_params._mode == mode_server){
	net_srv.start();
    }
}

void app::parse_command_line(int argc, char **argv){

    int c = 0;
    while ((c = getopt(argc, argv, "hp:m:a:s:n:")) != -1) {
	switch (c) {
	    case 'h':
		usage();
		exit(0);
	    case 'p':
		// protocol (tcp/udp)
		_params._proto = optarg;
		break;
	    case 'm':
		// mode (client/server)
		_params._mode = get_mode_value(optarg);
		break;
	    case 'a':
		// address (IP/DNS-name)
		_params._addr = optarg;
		break;
	    case 's':
		// service(port/service name)
		_params._service = optarg;
		break;
	    case 'n':
		// network nahdler
		_params._network_handler = optarg;
		break;

	    default:
		usage();
		exit(0);
	}
    }
}