#include <iostream>
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
 
template <typename T>
class threadsafe_queue{
public:
    threadsafe_queue(){}
    bool empty() const {
        std::lock_guard<std::mutex> l(_mutex);
        return _queue.empty();
    }
 
    void push(T new_value){
        std::lock_guard<std::mutex> l(_mutex);
        _queue.push_back(std::move(new_value));
        _cond.notify_one();
    }
 
    void wait_and_pop(T& value){
        std::unique_lock<std::mutex> l(_mutex);
        _cond.wait( l, [this]{return !_queue.empty();} );
        value = std::move(_queue.front());
        _queue.pop_front();
    }
 
    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock<std::mutex> l(_mutex);
        _cond.wait( l, [this]{return !_queue.empty();} );
        std::shared_ptr<T> res( std::make_shared(std::move(_queue.front())) );
        _queue.pop_front();
        return res;
    }
 
    bool try_pop(T& value){
        std::unique_lock<std::mutex> l(_mutex);
        if(_queue.empty())
            return false;
        value = std::move(_queue.front());
        _queue.pop_front();
        return true;
    }
 
    std::shared_ptr<T> try_pop(){
        std::unique_lock<std::mutex> l(_mutex);
        if(_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res( std::make_shared(std::move(_queue.front())) );
        _queue.pop_front();
        return res;
    }
 
private:
    mutable std::mutex _mutex;
    std::list<T> _queue;
    std::condition_variable _cond;
};
 
////////////////////////////////////////////////////
void add_queue_values(threadsafe_queue<int>& q){
    q.push(4);
    q.push(5);
    q.push(6);
}
 
void read_queue_values_wait1(threadsafe_queue<int>& q){
    while(!q.empty()){
        int i;
        q.wait_and_pop(i);
        std::cout << i << std::endl;
    }
}
 
void read_queue_values_wait2(threadsafe_queue<int>& q){
    int i;
    while(q.try_pop(i)){
        std::cout << i << std::endl;
    }
}
 
int main(int argc, char* argv[]) {
    
    try{
        threadsafe_queue<int> q;
        std::thread t1(add_queue_values, std::ref(q));
        q.push(1);
        q.push(2);
        q.push(3);
        t1.join();
        std::thread t2(read_queue_values_wait1, std::ref(q));
        t2.join();
    }
 
    catch(const std::exception& e){
 
    }
    return 0;
}

