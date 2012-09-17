#ifndef _async_threadpool_h_
#define _async_threadpool_h_

#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>


class async_threadpool{
public:

    async_threadpool(size_t threads_count = 0) 
        : io(), 
        asio_work(io),
        th_group(),
        _threads_count(threads_count){

            if( 0 ==_threads_count ){
                _threads_count = boost::thread::hardware_concurrency();
            }
    }

    void start(){

        for (size_t i = 0; i < _threads_count; ++i){
            th_group.create_thread(boost::bind(&boost::asio::io_service::run, &io));
        }
    }

    template <typename T>
    void post_task(T handler){
        io.post(handler);
    };

    void stop(){
        io.stop();
        th_group.join_all();
    }

private:

    // DO NOT CHANGE MEMBER INITIALIZATION ORDER!

    /// Asynchronous thread service
    boost::asio::io_service io;

    /// Asio work
    boost::asio::io_service::work asio_work;

    /// Pool of threads
    boost::thread_group th_group;

    /// 
    size_t _threads_count;
};


#endif //_async_threadpool_h_