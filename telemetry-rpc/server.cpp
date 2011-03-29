#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>

#include <server/TSimpleServer.h>
#include <server/TThreadPoolServer.h>
#include <server/TThreadedServer.h>

#include <transport/TServerSocket.h>
#include <transport/TTransportUtils.h>
#include <transport/TBufferTransports.h>

#include "./gen-cpp/UavTelemetry.h"
#include <protocol/TBinaryProtocol.h>


#include <iostream>
#include <stdexcept>
#include <sstream>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace boost;
using namespace shared;
using namespace telemetry_rpc;

class telemetry_server {
public:

    telemetry_server() :
    _exit_flag(false) {
        tret.uav_addr = 1;
        boost::thread t(&telemetry_server::message_loop, this);
    }

    void message_loop() {
        while (!_exit_flag) {
            tret.lattitude += 1.;
            tret.longitude += 1.;
            boost::this_thread::sleep(boost::posix_time::millisec(500));
        }
    }

    void set_pitch(int p) {
        boost::mutex::scoped_lock some_lock(lk);
        tret.pitch = p;
    }

    void set_roll(int p) {
        boost::mutex::scoped_lock some_lock(lk);
        tret.roll = p;
    }

    void set_elevator(int p) {
        boost::mutex::scoped_lock some_lock(lk);
        tret.elevator = p;
    }

    void set_rudder(int p) {
        boost::mutex::scoped_lock some_lock(lk);
        tret.rudder = p;
    }

    const Telemetry& get_telemetry() {
        return tret;
    }

private:
    Telemetry tret;
    bool _exit_flag;
    boost::mutex lk;
};

class UavTelemetryHandler : virtual public UavTelemetryIf {
public:

    UavTelemetryHandler() {
        // Your initialization goes here
    }

    void ping() {
        // Your implementation goes here
        printf("ping\n");
    }

    void get_telemetry(Telemetry& _return) {
        // Your implementation goes here
        printf("get_telemetry\n");
        _return = s.get_telemetry();
    }

    void getStruct(SharedStruct &ret, const int32_t logid) {
        printf("getStruct(%d)\n", logid);
        ret = log[logid];
    }

    virtual void set_pitch(const int32_t p) {
        printf("set_pitch = %d\n", p);
        s.set_pitch(p);
    }

    virtual void set_roll(const int32_t p) {
        printf("set_roll = %d\n", p);
        s.set_roll(p);
    }

    virtual void set_elevator(const int32_t p) {
        printf("set_elevator = %d\n", p);
        s.set_elevator(p);
    }

    virtual void set_rudder(const int32_t p) {
        printf("set_rudder = %d\n", p);
        s.set_rudder(p);
    }


protected:
    map<int32_t, SharedStruct> log;
    telemetry_server s;
};

int main(int argc, char **argv) {

    int port = 9090;
    shared_ptr<UavTelemetryHandler> handler(new UavTelemetryHandler());
    shared_ptr<TProcessor> processor(new UavTelemetryProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

#if 0
    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
#else
    TThreadedServer server(processor,
            serverTransport,
            transportFactory,
            protocolFactory);
    server.serve();
#endif

    return 0;

#if 0
    TSimpleServer server(processor,
            serverTransport,
            transportFactory,
            protocolFactory);
#endif

    /**
     * Or you could do one of these

    shared_ptr<ThreadManager> threadManager =
      ThreadManager::newSimpleThreadManager(workerCount);
    shared_ptr<PosixThreadFactory> threadFactory =
      shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();
    TThreadPoolServer server(processor,
                             serverTransport,
                             transportFactory,
                             protocolFactory,
                             threadManager);

    TThreadedServer server(processor,
                           serverTransport,
                           transportFactory,
                           protocolFactory);

     */

    //printf("Starting the server...\n");
    //server.serve();
    //printf("done.\n");
    return 0;
}

