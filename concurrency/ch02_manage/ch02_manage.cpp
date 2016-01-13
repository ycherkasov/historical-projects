#include <iostream>
#include <cstdlib>
#include <thread>
#include <algorithm>
#include <stdexcept>

// Concurrency in Action. Chapter 2 
// Manage threads

// * std::thread do accept any that work as a function:
//- global function
//- method
//- functor
//- lambda
// * detached thread could not be attached (joined) again
//   this is useful for background non-interactive tasks
// * join() makes caller-thread wait for called thread
// * In case of doing some work before join you should take care about exceptions
// * Exception being thrown could skip join() call
//   You should join() in catch(...) section or use RAII
// * only joinable() threads (not detached nor joined) could be joined
// * Arguments passed to the thread are COPIED to the thread context
// * In this case important to pass parameters without implicit constructors call
//   therefore caller function could exit before the construction finishes
// * hardware_concurrency could be used for pooling


// Examples:
// 1. Plain thread function
// 2. Lambda thread
// 3. Error! Stack usage
// 4. Thread guard
// 5. Error! Implicit constructor usage
// 6. Passing by ref
// 7. hardware_concurrency
// 8. Thread ID

void show_functor_thread(){
    
    struct background_task{
        void operator()() const {
            // do some background work
            double i = 0.0;
            while (i < 10000.0){
                i += rand();
            }
        }
    };

    std::thread t(background_task{});
    // detached thread could not be attached (joined) again
    // this is useful for background non-interactive tasks
    t.detach();
}


void show_lambda_thread(){
    std::thread lambda_thread([]{ 
        std::cout << "Threaded lambda function" << std::endl;
    });
    // join() makes caller-thread wait for called thread
    lambda_thread.join();
}

// You should guarantee that all data that thread can request
// will be available until it will finish

// Don't do that!
#if 0
void oops(){
    int ext = 10;
    struct access_ext{
        operator()(int& ext){
            // do something with ext
        }
    };
    std::thread t(access_ext, ext);
    // Could be destroyed before t finish his work!
    t.detach();
}
#endif


// In case of doing some work before join you should take care about exceptions
// Exception being thrown could skip join() call
// You should join() in catch(...) section or use RAII

// RAII wrapper for the joinable thread
class thread_guard{
public:

    // move from rvalue
    explicit thread_guard(std::thread t) : _t(std::move(t)){
        
        if (!_t.joinable()) {
            throw std::logic_error("Unable to join in thread_guard");
        }
    }

    // Join in the destructor - will be joined in case of exception
    ~thread_guard(){
        // only joinable threads (not detached nor joined) could be joined
        if(_t.joinable()){
            _t.join();
        }
    }
    
    // not copyable/assignable
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;
private:
    std::thread _t;
};


void show_thread_guard(){
    
    auto f = []{ 
        std::cout << "RAII lambda function" << std::endl;
    };
    thread_guard tg(std::thread(f));
}

// Arguments passed to the thread are COPIED to the thread context
// In this case important to pass parameters without implicit constructors call
// therefore caller function could exit before the construction finishes

// Don't do that!
#if 0
void oops2(){
        
    struct argumented_task{
        void operator(int i, std::string s)() const {
            // use passed variables
            std::string s1 = s;
            double d = static_cast<double>(i) + static_cast<double>(s1.size());
            while (d < 10000.0){
                d += rand();
            }
        }
    };

    std::thread t(argumented_task{3, "Hello"});
    // Don't do that! oops2() could finish before std::string("Hello") construction
    // is being finished. Call 
    // std::thread t(argumented_task{3, std::string("Hello")});
    // instead. It will call string() constructor before the thread starts
    t.detach();
}
#endif

void thread_func_result(int up_to, int& res) {
    int i = 0;
    for (; i < up_to; ++i) {}
    res = i;
}

// To pass th the thread context not a copy, but reference 
// (for the shared access to it) std::ref() call could be used
void show_passing_by_ref(){

    // if return resuls by reference, pass it explicitly
    int res{};
    std::thread t3{ thread_func_result, 10000000, std::ref(res) };
    t3.join();
}

size_t threads_per_dataset(size_t dataset_size) {

    // not to create threads for the small amount of data
    constexpr size_t min_per_thread = 25;
    const size_t max_threads = (dataset_size + min_per_thread - 1) / min_per_thread;

    unsigned int hardware_threads = std::thread::hardware_concurrency();

    return std::min(hardware_threads ? hardware_threads : 2, max_threads);
}

void show_hardware_concurrency() {
    std::cout << "5 elements = " << threads_per_dataset(5) << std::endl;
    std::cout << "24 elements = " << threads_per_dataset(24) << std::endl;
    std::cout << "25 elements = " << threads_per_dataset(25) << std::endl;
    std::cout << "50 elements = " << threads_per_dataset(50) << std::endl;
    std::cout << "5000 elements = " << threads_per_dataset(5000) << std::endl;
}

void show_thread_id() {
    // thread_id format is system-specific
    std::cout << std::this_thread::get_id() << std::endl;
}

int main(int argc, char* argv[]) {
    
    try{
        show_functor_thread();
        show_lambda_thread();
        show_thread_guard();
        show_hardware_concurrency();
        show_thread_id();
    }
    catch(const std::exception& e){
    }
    return 0;
}
