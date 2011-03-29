#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include <iostream>
#include <boost/thread.hpp>

#include "./gen-cpp/UavTelemetry.h"
#include "gen-cpp/UavTelemetry.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace telemetry_rpc;
using namespace shared;

using namespace boost;

int main(int argc, char** argv) {

    bool set_mode = false;
    if (argc == 2) {
        set_mode = true;
    }

    shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
    shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    UavTelemetryClient client(protocol);

    try {
        transport->open();
        client.ping();
        printf("ping()\n");

        if (set_mode) {
            for (int i = 0; i < 1000; i++) {
                client.set_roll(i);
                client.set_pitch(i + 3);
                client.set_rudder(i + 1);
                client.set_elevator(i + 10);
                boost::this_thread::sleep(boost::posix_time::millisec(50));
            }

        }
        else {
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

        }

        transport->close();
    }
    catch (TException &tx) {
        printf("ERROR: %s\n", tx.what());
    }

}

