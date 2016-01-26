#include <iostream>
#include <stdexcept>
#include <sstream>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>

#include <server/TSimpleServer.h>
#include <server/TThreadPoolServer.h>
#include <server/TThreadedServer.h>

#include <transport/TServerSocket.h>
#include <transport/TTransportUtils.h>
#include <transport/TBufferTransports.h>

#include <protocol/TBinaryProtocol.h>

#include "./gen-cpp/UavTelemetry.h"

using namespace shared;
using namespace telemetry_rpc;

boost::mt19937 gen;

/** @brief Generate random int in a provided range */
int rand_int(int from, int to) {
    boost::uniform_int<> dist(from, to);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);
    return die();
}

/** @brief Dummy UAV state pattern which stores last telemetry stamp */
class telemetry_server {
public:

    telemetry_server() :
    _exit_flag(false) {
        tret.uav_addr = 1;
        boost::thread t(&telemetry_server::message_loop, this);
    }

    void message_loop() {
        while (!_exit_flag) {
            tret.lattitude += rand_int(-6, 6);
            tret.longitude += rand_int(-6, 6);
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

/** @brief RPC server interface implementation */
class UavTelemetryHandler : virtual public UavTelemetryIf {
public:

    UavTelemetryHandler() { }

    void ping() { }

    void get_telemetry(Telemetry& _return) {
        _return = s.get_telemetry();
    }

    void getStruct(SharedStruct &ret, const int32_t logid) {
        ret = log[logid];
    }

    virtual void set_pitch(const int32_t p) {
        std::cout << "set_pitch = " << p << std::endl;
        s.set_pitch(p);
    }

    virtual void set_roll(const int32_t p) {
        std::cout << "set_roll = " << p << std::endl;
        s.set_roll(p);
    }

    virtual void set_elevator(const int32_t p) {
        std::cout << "set_elevator = " << p << std::endl;
        s.set_elevator(p);
    }

    virtual void set_rudder(const int32_t p) {
        std::cout << "set_rudder = " << p << std::endl;
        s.set_rudder(p);
    }

    virtual void save() {
        std::cout << "save async method " << std::endl;
    }

protected:
    std::map<int32_t, SharedStruct> log;
    telemetry_server s;
};

void help() {
    std::cerr << "Usage: server [single|threaded|threadpool]\n";
    exit(0);
}

int main(int argc, char **argv) {

    using namespace apache::thrift;
    using namespace apache::thrift::protocol;
    using namespace apache::thrift::transport;
    using namespace apache::thrift::server;
    using namespace apache::thrift::concurrency;

    if (argc != 2) {
        help();
    }
    std::string mode(argv[1]);

    int port = 9090;
    boost::shared_ptr<UavTelemetryHandler> handler(new UavTelemetryHandler());
    boost::shared_ptr<TProcessor> processor(new UavTelemetryProcessor(handler));
    boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    std::cout << "Starting the server at port 9090..." << std::endl;

    if (0 == mode.compare("single")) {

        TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
        server.serve();

    }
    else if (0 == mode.compare("threaded")) {

        TThreadedServer server(processor,
                serverTransport,
                transportFactory,
                protocolFactory);
        server.serve();

    }
    else if (0 == mode.compare("threadpool")) {

        // threads count in pool
        const size_t workerCount = 5;
        boost::shared_ptr<ThreadManager> threadManager =
                ThreadManager::newSimpleThreadManager(workerCount);

        // create POSIX-specific thread factory
        boost::shared_ptr<PosixThreadFactory> threadFactory(new PosixThreadFactory());

        // bind thread factory to thread manager
        threadManager->threadFactory(threadFactory);
        threadManager->start();
        TThreadPoolServer server(processor,
                serverTransport,
                transportFactory,
                protocolFactory,
                threadManager);
        server.serve();
    }
    else {
        help();
    }

    return 0;
}

