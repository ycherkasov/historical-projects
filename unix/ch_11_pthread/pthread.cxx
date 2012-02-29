#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <stdexcept>

/*
 * Chapter 11 Stievens, Rago. Pthread.
 * 1. Use pthread_self() to get current tid, pthread_equal() to campare tids
 *    (because pthread_t can be not basic type only).
 * 2. After pthread_create() call order (parent tread-child thread) is undefined.
 * 3. main() thread must wait for other threads in pthread_join(),
 *    or process will be finished instead.
 * 4. pthread_exit() returns value to pthread_join() output parameter or whole
 *    structure (its visibility scope must be wider than thread function)
 * 5. pthread_cancel() ask thread to finish (it is async call)
 * 6. pthread_cleanup_push/pop() pair can assign exit handler for thread
 *    which has been cancelled.
 * 7. Multiple threads can work only with atomic data types without
 *    syncronisation(int is not atomic!)
 * 8. Static mutexes could be ibitialized with PTHREAD_MUTEX_INITIALIZER
 * 9. We can check mutex state with pthread_mutex_trylock()
 * 10.pthread_mutex_trylock() can be used to prevent deadlocks
 * 11.Strict initialization order can be used also for deadlock prevention.
 * 12.rw_lock can lock thread for reading (multiple read threads can be perfomed
 *    , write blocks wait for last read finish),and for writing
 *    (only one writer allowed, all read and write calls blocked)
 * 13.Condition variables can be used for locks between threads (see examples)
 *
 * See also:
 * http://www.intuit.ru/department/se/posixthreads/7/
 * https://computing.llnl.gov/tutorials/pthreads/
 * http://en.wikipedia.org/wiki/Futex
 *
 */

using std::string;
using std::cout;
using std::cerr;
using std::endl;

typedef void *(thread_func)(void*);

/** @brief Thread function shows thread id operations */
void* compare_threads(void*) {

    // get current thread pid
    pthread_t self = pthread_self();
    cout << "Curent thread tid = " << self << endl;

    // do not compare threads with ==, use pthread_equal()
    std::string s;
    (pthread_equal(self, 0)) ? (s.assign("true")) : (s.assign("false"));
    cout << "pthread_equal to 0 = " << s << endl;

    // return value 1 to pthread_join()
    pthread_exit(reinterpret_cast<void*>(1));
}

/** @brief Thread cancel completeion handler */
void cleanup(void* arg) {
    cout << "Curent thread tid = " << pthread_self() << endl;
}

const char* first_handler = "first handler";
const char* second_handler = "first handler";

/** @brief Assign completion handlers for thread */
void* thread_with_cleanup(void* arg) {
    pthread_cleanup_push(cleanup, NULL);
    pthread_cleanup_push(cleanup, NULL);
    if (arg) {
        return reinterpret_cast<void*>(1);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return reinterpret_cast<void*>(0);
}

/** @brief RAII for thread (very incomplete, see more functional in Ch 12) */
class pthread_wrapper_t {
public:

    pthread_wrapper_t(thread_func func, void* arg = NULL) {
        pthread_create(&_tid, NULL, func, arg);
    }

    void* join() {
        void* r;
        pthread_join(_tid, &r);
        return r;
    }

    void cancel() {
        pthread_cancel(_tid);
    }

    void detach() {
        pthread_detach(_tid);
    }

private:
    pthread_t _tid;
};

/** @brief RAII for mutex */
class mutex_wrapper_t {
    friend class cond_wrapper_t;
public:

    mutex_wrapper_t() {
        if (0 != pthread_mutex_init(&_mutex, NULL)) {
            throw std::runtime_error(strerror(errno));
        }
    }

    ~mutex_wrapper_t() {
        pthread_mutex_destroy(&_mutex);
    }

    void lock() {
        pthread_mutex_lock(&_mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&_mutex);
    }

    bool try_lock() {
        pthread_mutex_trylock(&_mutex);
    }

private:
    pthread_mutex_t _mutex;
};

/** @brief RAII for RWLock */
class rw_lock_wrapper_t {
public:

    rw_lock_wrapper_t() {
        pthread_rwlock_init(&_lock, NULL);
    }

    ~rw_lock_wrapper_t() {
        pthread_rwlock_destroy(&_lock);
    }

    void read_lock() {
        pthread_rwlock_rdlock(&_lock);
    }

    void write_lock() {
        pthread_rwlock_wrlock(&_lock);
    }

    void lock() {
        write_lock();
    }

    void unlock() {
        pthread_rwlock_unlock(&_lock);
    }

private:
    pthread_rwlock_t _lock;
};

/** @brief RAII for condition variable */
class cond_wrapper_t {
    
public:
    cond_wrapper_t(){
        pthread_cond_init(&_cond, NULL);
    }

    ~cond_wrapper_t(){
        pthread_cond_destroy(&_cond);
    }

    void wait(mutex_wrapper_t& m){
        pthread_cond_wait(&_cond, &(m._mutex));
    }

    void timedwait(mutex_wrapper_t& m, const struct timespec& t){
        pthread_cond_timedwait(&_cond, &(m._mutex), &t);
    }

    void signal(){
        pthread_cond_signal(&_cond);
    }

    void broadcast(){
        pthread_cond_broadcast(&_cond);
    }

private:
    pthread_cond_t _cond;
};

/** @brief RAII for lock (lock all as mutex or write lock) */
template <typename T>
class scoped_lock_t {
public:

    scoped_lock_t(T& obj) : _object(obj) {
        _object.lock();
    }

    ~scoped_lock_t() {
        _object.unlock();
    }

private:
    T& _object;
};

/** @brief RAII for read lock (RWLock only) */
template <typename T>
class scoped_read_lock_t {
public:

    scoped_read_lock_t(T& obj) : _object(obj) {
        _object.read_lock();
    }

    ~scoped_read_lock_t() {
        _object.unlock();
    }

private:
    T& _object;
};


// ------------ Condition variable example ------------
// Producer - multi-consumer model

/** @brief Long operatiopn to prodice some data */
int produce(){
    int ret = 1;
    sleep(ret);
    return ret;
}

/** @brief Long operatiopn to process data fetched from prdusser */
void consume(int data){
    sleep(data);
}

// Global data, shared between prducer and consumer threads
mutex_wrapper_t g_mutexm;
cond_wrapper_t g_cond;
bool consumer_busy;
bool producer_busy;
int shared_data;

/** @brief Producer-concumer example: Thread function produces data
 * and send it througth shared storage to multiple concumers */
void* producer_thread(void*) {

    // producer starts first, so it's busy
    producer_busy = true;
    while(true){

        
        // produce some data
        int s = produce();

        // lock until consumer get ready
        scoped_lock_t<mutex_wrapper_t> l(g_mutexm);
        cout << "Producer: Produced data = " << shared_data << endl;
        cout << "Producer: locked" << endl;

        // wait for consumer (using while instead of if is importaint!)
        while(consumer_busy){
            cout << "Producer: Wait while consumer finish with data" << endl;
            g_cond.wait(g_mutexm);
        }
        cout << "Producer: Consumer is ready, pass data to shared" << endl;

        // copy produced data to shared storage
        shared_data = s;

        // tell concumer he's busy
        producer_busy = false;
        consumer_busy = true;

        cout << "Produser: send signal (data is ready, copy it)" << endl;
        g_cond.broadcast();
    }

}

/** @brief Producer-concumer example: Get data from shared storage  */
void* consumer_thread(void* no) {
    consumer_busy = false;
    long consumer_id = reinterpret_cast<long>(no);
    while(true) {

        int local_data = 0;
        do {
            scoped_lock_t<mutex_wrapper_t> l(g_mutexm);
            cout << "Consumer id = " << consumer_id << ": locked" << endl;

            while (producer_busy) {
                cout << "Consumer id = " << consumer_id
                << ": Wait while produser prepares new data" << endl;
                g_cond.wait(g_mutexm);
            }

            cout << "Consumer id = " << consumer_id
                << ": Data prodused, copy and process it" << endl;
            local_data = shared_data;
            producer_busy = true;
            consumer_busy = false;

            cout << "Consumer id = " << consumer_id
                << ": send signal (data copied, give me more)" << endl;
            g_cond.signal();
        }
        while (0);
        consume(local_data);
    }
 }

/** @brief Producer-concumer example: start threads */
int start_producer_consumer() {

    // create one producer and two consumers
    pthread_wrapper_t ptw(producer_thread);
    pthread_wrapper_t ctw1(consumer_thread, reinterpret_cast<void*>(1) );
    pthread_wrapper_t ctw2(consumer_thread, reinterpret_cast<void*>(2) );
    ptw.join();
    ctw1.join();
    ctw2.join();
    return 0;
}

// ------------ Condition variable example ------------
// Multi-producer - watchdog model
const int NUM_THREADS = 3;
const int TCOUNT = 10;
const int COUNT_LIMIT = 12;
int     count = 0;
int     thread_ids[3] = {0,1,2};
mutex_wrapper_t  count_mutex;
cond_wrapper_t count_threshold_cv;

/** @brief Producers-watchdog example: multiple increment threads */
void* inc_count(void *t) {
    long my_id = (long)t;

    for (int i = 0;i < TCOUNT;i++) {

        do {
            scoped_lock_t<mutex_wrapper_t> l(count_mutex);
            count++;

            /* Check the value of count and signal waiting thread when condition is
             * reached.  Note that this occurs while mutex is locked.
             */
            if (count == COUNT_LIMIT) {
                count_threshold_cv.signal();
                cout << "inc_count(): thread " << my_id <<
                ", count = " << count <<
                "  Threshold reached" << endl;
            }
            cout << "inc_count(): thread " << my_id <<
            ", count = " << count <<
            ", unlocking mutex" << endl;

        }
        while (0);
        /* Do some "work" so threads can alternate on mutex lock */
        sleep(1);
    }
    pthread_exit(NULL);
}

/** @brief Producers-watchdog example: the only watchdog thread */
void *watch_count(void *t) {
    long my_id = (long)t;

    cout << "Starting watch_count(): thread " << my_id << endl;

    /* Lock mutex and wait for signal.  Note that the pthread_cond_wait
     * routine will automatically and atomically unlock mutex while it waits.
     * Also, note that if COUNT_LIMIT is reached before this routine is run by
     * the waiting thread, the loop will be skipped to prevent pthread_cond_wait
     * from never returning.
     */
    scoped_lock_t<mutex_wrapper_t> l(count_mutex);
    while (count < COUNT_LIMIT) {
        count_threshold_cv.wait(count_mutex);
        cout << "watch_count(): thread " <<  my_id <<
        " Condition signal received" << endl;
        count += 125;
        cout << "watch_count(): thread " << my_id <<
        " count now = " << count << endl;
    }
    pthread_exit(NULL);
}

/** @brief Producers-watchdog example: start threads */
int start_count_process() {
    long t1 = 1, t2 = 2, t3 = 3;
    pthread_t threads[3];

    /* For portability, explicitly create threads in a joinable state */
    pthread_create(&threads[0], NULL, watch_count, (void *)t1);
    pthread_create(&threads[1], NULL, inc_count, (void *)t2);
    pthread_create(&threads[2], NULL, inc_count, (void *)t3);

    /* Wait for all threads to complete */
    for (int i = 0 ; i < NUM_THREADS ; i++) {
        pthread_join(threads[i], NULL);
    }
    cout << "Main(): Waited on threads" << endl;

    /* Clean up and exit */
    return 0;
}


int main() {

    // get information about current thread
#if 0
    compare_threads(NULL);
#endif

    // start new thread and get information about it
#if 0
    pthread_wrapper_t ptw(compare_threads);

    // wait until it finishes
    void* tret = ptw.join();
    cout << "Thread return code " << tret << endl;
#endif

    // show thread cleanup procedure
#if 0
    pthread_wrapper_t ptw1(thread_with_cleanup);
    ptw1.join();
#endif
    
    // condition variable usage
#if 0
    // show multi-consumer one-watcher
    start_count_process();
#else
    // show produser-concumer pattern with mutex+cond variable
    start_producer_consumer();
#endif

    return 0;
}
