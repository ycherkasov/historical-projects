#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "gen-cpp/UavTelemetry.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace telemetry_rpc;
using namespace shared;

void help() {
    std::cerr << "Usage: cleant [get|set]\n";
    exit(0);
}

boost::mt19937 gen;

/** @brief Generate random int in a provided range */
int rand_int(int from, int to) {
    boost::uniform_int<> dist(from, to);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);
    return die();
}

void get_rpc_telemetry(UavTelemetryClient& client) {
    while (true) {
        Telemetry t;
        client.get_telemetry(t);
        std::cout << "Addr: " << static_cast<int> (t.uav_addr) << std::endl;
        std::cout << "Pitch: " << t.pitch << std::endl;
        std::cout << "Roll: " << t.roll << std::endl;
        std::cout << "Latt: " << t.lattitude << std::endl;
        std::cout << "Long: " << t.longitude << std::endl;

        SharedStruct ss;
        client.getStruct(ss, 1);
        printf("Check log: %s\n", ss.value.c_str());
        boost::this_thread::sleep(boost::posix_time::millisec(500));
    }
}

void set_rpc_telemetry(UavTelemetryClient& client) {
    while (true) {
        client.set_roll(rand_int(-127, 127));
        client.set_pitch(rand_int(-127, 127));
        client.set_rudder(rand_int(-127, 127));
        client.set_elevator(rand_int(-127, 127));

        boost::this_thread::sleep(boost::posix_time::millisec(500));
    }

}

class transport_wrapper {
private:
    TTransport* transport;
public:

    transport_wrapper(TTransport* t) : transport(t) {
        transport->open();
    }

    ~transport_wrapper() {
        transport->close();
    }
};

int main(int argc, char** argv) {


    if (argc != 2) {
        help();
    }
    std::string mode(argv[1]);

    boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    UavTelemetryClient client(protocol);


    try {
        transport_wrapper t(transport.get());

        // check connection
        client.ping();
        printf("ping()\n");

        if (0 == mode.compare("get")) {
            get_rpc_telemetry(client);
        }
        else if (0 == mode.compare("set")) {
            set_rpc_telemetry(client);
        }
        else {
            help();
        }

    }
    catch (TException &tx) {
        printf("ERROR: %s\n", tx.what());
    }

}

