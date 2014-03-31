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
 
private:
    std::condition_variable _cond;
    mutable std::mutex _mutex;
    std::list< std::unique_ptr<T> > _queue;
    std::atomic<size_t> _counter;
    const size_t _max_counter;
};
 
 
static SharedQueue<int> q(128);
 
 
////////////////////////////////////////////////////
void add_queue_values(){
    srand (time(NULL));
    int counter = 0;
    while (true)
    {
        std::cout << counter << '\n';
        int* i = new int( rand() );
        q.enqueue(i);
        ++counter;
    }
}
 
void read_queue_values(){
    int counter = 0;
    while (true)
    {
        std::cout << counter << '\n';
        int* i = q.dequeue();
        delete i;
        --counter;
    }
}

 
int main(int argc, char* argv[]) {
    
    try{
        std::thread t1(add_queue_values);
        std::thread t2(read_queue_values);
        t1.join();
        t2.join();
    }
    catch(const std::exception& e){
    }
    return 0;
}


#if 0
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
            return false;
        }
 
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
            return res;
        }
        else
        {
            return nullptr;
        }
    }

#endif
