#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <stdexcept>


using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 11 Stievens, Rago. Pthread.
 * 1.Use pthread_self() to get current tid, pthread_equal() to campare tids.
 */

void* compare_threads(void*){

    // get current thread pid
    pthread_t self = pthread_self();
    cout << "Curent thread tid = " << self << endl;

    // do not compare threads with ==, use pthread_equal()
    std::string s;
    (pthread_equal(self, 0))?(s.assign("true")):(s.assign("false"));
    cout << "pthread_equal to 0 = " << s << endl;

    return reinterpret_cast<void*>(1);
}


void cleanup(void* arg){
    cout << "Curent thread tid = " << pthread_self() << endl;
    //cout<< "exit: " << (char *)arg << endl;
}

const char* first_handler = "first handler";
const char* second_handler = "first handler";

void* thread_with_cleanup(void* arg){
    pthread_cleanup_push(cleanup, NULL);
    pthread_cleanup_push(cleanup, NULL);
    if(arg){
        return reinterpret_cast<void*>(1);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return reinterpret_cast<void*>(0);
}

typedef void *(thread_func)(void*);

class pthread_wrapper_t{
public:

    pthread_wrapper_t(thread_func func){
        //pthread_create(&_tid, _func, &_attr, 0);
        pthread_create(&_tid, NULL, func, 0);
    }

    void* join(){
        void* r;
        pthread_join(_tid, &r);
        return r;
    }

    void cancel(){
        pthread_cancel(_tid);
    }

    void detach(){
        pthread_detach(_tid);
    }

private:
    pthread_attr_t _attr;
    pthread_t _tid;
};

class mutex_wrapper_t{
public:
    mutex_wrapper_t(){
        if( 0 != pthread_mutex_init(&_mutex, NULL) ){
            throw std::runtime_error( strerror(errno) );
        }
    }

    ~mutex_wrapper_t(){
        pthread_mutex_destroy(&_mutex);
    }

    void lock(){
        pthread_mutex_lock(&_mutex);
    }

    void unlock(){
        pthread_mutex_unlock(&_mutex);
    }

    bool try_lock(){
        pthread_mutex_trylock(_mutex);
    }

private:
    pthread_mutex_t _mutex;
};

class scoped_lock_t{
    scoped_lock_t(mutex_wrapper_t& mw):_mutex(mw){
        _mutex.lock();
    }

    ~scoped_lock_t(){
        _mutex.unlock();
    }

private:
    mutex_wrapper_t& _mutex;
};

class rw_lock_wrapper_t{
public:
    rw_lock_wrapper_t(){
        pthread_rwlock_init(&_lock, NULL);
    }

    ~rw_lock_wrapper_t(){
        pthread_rwlock_destroy(&_lock);
    }

    void read_lock(){
        pthread_rwlock_rdlock(&_lock);
    }

    void write_lock(){
        pthread_rwlock_wrlock(&_lock);
    }

    void unlock(){
        pthread_rwlock_unlock(&_lock);
    }

private:
    pthread_rwlock_t _lock;
};

int main(){
    // get information about current thread
    compare_threads(NULL);

    // start new thread and get information about it
    pthread_wrapper_t ptw(compare_threads);

    // wait until it finishes
    void* tret = ptw.join();
    cout << "Thread return code " << tret << endl;

    pthread_wrapper_t ptw1(thread_with_cleanup);
    ptw1.join();


    return 0;
}
