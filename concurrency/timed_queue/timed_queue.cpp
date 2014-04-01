#include <iostream>
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
 
 
/// Thread-safe fixed-size shared queue with low 'empty queue' latency.
/// @note The queue 'owns' the elements that it contains.
template <typename T> 
class SharedQueue
{
private:
    // note: MSVC 11 does not support '= delete' notation
    SharedQueue(const SharedQueue&);
    SharedQueue& operator=(const SharedQueue&);
public:
 
    /// Constructor.
    /// @param size Queue size.
    SharedQueue(size_t size):_counter(), _max_counter(size){}
 
    /// Returns the number of elements contained in the Queue.        
    int count() const{
        return _counter.load();
    }
 
    /// Puts the item into the queue. 
    /// @note if the queue is full then this method blocks until there is the room for the item again.
    void enqueue(T* item){
        std::unique_lock<std::mutex> l(_mutex);
        _cond.wait( l, [this]{return (_counter.load() < _max_counter);} );
        _queue.push_back( std::unique_ptr<T>(item) );
        ++_counter;
        _cond.notify_one();
    }
 
 
    /// Puts the item into the queue. 
    /// @param millisecondsTimeout Numbers of milliseconds to wait.
    /// @return 'true' if the operation was completed successfully, 'false' if the operation timed out.
    /// @note if the queue is full then this method blocks until there is the room for the item again or the operation timed out.
    bool enqueue(T* item, int millisecondsTimeout){
        std::unique_lock<std::mutex> l(_mutex);
        if( _cond.wait_for( l, 
            std::chrono::milliseconds(millisecondsTimeout),
            [this]{return (_counter.load() < _max_counter);} ) )
        {
            _queue.push_back( std::unique_ptr<T>(item) );
            ++_counter;
            _cond.notify_one();
            return true;
        }
        else
        {
            _cond.notify_one();
            return false;
        }
 
    }
 
    /// Removes and returns the item at the beginning of the Queue.
    /// @note if the queue is empty then this method blocks until there is an item again.
    T* dequeue(){
        std::unique_lock<std::mutex> l(_mutex);
        _cond.wait( l, [this]{return _counter.load() > 0;} );
        T* res = _queue.front().release();
        _queue.pop_front();
        --_counter;
        _cond.notify_one();
        return res;
    }
 
    /// Removes and returns the item at the beginning of the Queue. 
    /// @param millisecondsTimeout Numbers of milliseconds to wait.
    /// @returns The item at the betting of the Queue or NULL if the operation timed out.	
    /// @note if the queue is empty then this method blocks until there is an item again or the operation timed out.
    T* dequeue(int millisecondsTimeout){
        std::unique_lock<std::mutex> l(_mutex);
        if( _cond.wait_for( l, 
            std::chrono::milliseconds(millisecondsTimeout),
            [this]{return _counter.load() > 0;} ) )
        {
            T* res = _queue.front().release();
            _queue.pop_front();
            --_counter;
            _cond.notify_one();
            return res;
        }
        else
        {
            _cond.notify_one();
            return nullptr;
        }
    }
 
private:
    std::condition_variable _cond;
    mutable std::mutex _mutex;
    std::list< std::unique_ptr<T> > _queue;
    std::atomic<size_t> _counter;
    const size_t _max_counter;
};
 
 
template <typename T>
SharedQueue<T>& getSharedQueue(size_t queueSize){
    static SharedQueue<T> q(queueSize);
    return q;
}
 
/////////////////////////////////////////////////////
// TEST ITEMS
 
struct TestItem
{
    TestItem(int i):_x(i), _y(rand()), _z(){};
    int _x;
    long _y;
    double _z;
};
 
 
struct HeavyItem
{
    HeavyItem(int i):_i(i),_array(){}
        int _i;
        long _array[255];
};
 
 
////////////////////////////////////////////////////
// LOAD TESTS 1
template <typename T>
void add_queue_values(SharedQueue<T>& q){
    srand (static_cast<unsigned>(time(NULL)));
    int counter = 0;
    while (true)
    {
        std::cout << ' ' << counter << '\n';
        T* i = new T( rand() );
        q.enqueue(i);
        ++counter;
    }
}
 
template <typename T>
void read_queue_values(SharedQueue<T>& q){
    int counter = 0;
    while (true)
    {
        std::cout << counter << '\n';
        T* i = q.dequeue();
        delete i;
        --counter;
    }
}
 
////////////////////////////////////////////////////
// LOAD TESTS 2
template <typename T>
void add_queue_values_timed(SharedQueue<T>& q){
    srand (static_cast<unsigned>(time(NULL)));
    int counter = 0;
    while (true)
    {
        std::cout << "W " << counter << '\n';
        T* i = new T( rand() );
        if(!q.enqueue(i, 1))
            std::cout << "Fail to push at step " << counter << '\n';
        ++counter;
    }
}
 
template <typename T>
void read_queue_values_timed(SharedQueue<T>& q){
    int counter = 0;
    while (true)
    {
        std::cout << "R" << counter << '\n';
        T* i = nullptr;
        if((i = q.dequeue(1)) == nullptr)
            std::cout << "Fail to pop at step " << counter << '\n';
        delete i;
        --counter;
    }
}
 
template <typename T>
void testQueueBasic(size_t queueSize){
    SharedQueue<T>& q = getSharedQueue<T>(queueSize);
    std::thread t1(add_queue_values<T>, std::ref(q));
    std::thread t2(read_queue_values<T>, std::ref(q));
    t1.join();
    t2.join();
}
 
template <typename T>
void testQueueTimed(size_t queueSize){
    SharedQueue<T>& q = getSharedQueue<T>(queueSize);
    std::thread t1(add_queue_values_timed<T>, std::ref(q));
    std::thread t2(read_queue_values_timed<T>, std::ref(q));
    t1.join();
    t2.join();
}
 
int main(int argc, char* argv[]) {
    
    try{
        testQueueTimed<int>(128);
        //testQueueTimed<long long>(128);
        //testQueueTimed<double>(128);
        //testQueueTimed<TestItem>(128);
        //testQueueTimed<HeavyItem>(128);
    }
    catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }
    return 0;
}