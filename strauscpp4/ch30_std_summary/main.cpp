#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>
#include <thread>
#include <future>

using namespace std;

/*

New features:
* Copying and rethrowing exceptions (30.4.1.2)
* Error reporting using error codes: system_error (30.4.3)

Examples:
1. Error propagation basic
2. Error propagation multithread
3. packaged task propagation
4. system_error example with file
5. system_error + error_category
6. system_error + error_condition
*/

// 1. Error propagation basic
namespace cpp4 {


// pass exception_ptr by value
void propagate_exception(std::exception_ptr eptr){
    
    // check whether exception is exists
    if (eptr) {
        std::rethrow_exception(eptr);
    }
}

void wrap_exception() {
    // std::exception_ptr is a nullable pointer-like type that manages an exception object 
    // which has been thrown and captured with std::current_exception
    std::exception_ptr ex_ptr;

    // std::make_exception_ptr also creates ritht from exception object

    try {
        std::string s;
        const char c = s.at(1); // throw here
    }
    catch (...) {
        // hold thrown exception in ptr
        ex_ptr = std::current_exception();
    }

    cpp4::propagate_exception(ex_ptr);
}

} // namespace cpp4 

void show_exception_propagation() {

    try{
        cpp4::wrap_exception();
    }
    catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
}


// 2. Error propagation multithread
namespace cpp4 {

    // bad example
    std::promise<int> global_promise;

void thread_propagate_excetion(){
    
    std::vector<int> v;
    int hhh{};

    try{
        hhh = v.at(1);
        global_promise.set_value(hhh);
    }
    catch (...){
        std::exception_ptr ex_ptr = std::current_exception();
        global_promise.set_exception(ex_ptr);  // does set_exception may throw?
    }
}

int thread_throw_excetion() {
    std::vector<int> v;
    int hhh{};

    hhh = v.at(1);
    return hhh;
}

} // namespace cpp4 


//4. system_error example with file
//5. system_error + error_category
//6. system_error + error_condition


void show_multithreaded_propagation() {

    std::thread t{ cpp4::thread_propagate_excetion };

    // instead of future current exception is thrown
    try{
        auto f = cpp4::global_promise.get_future();
        int result = f.get();
    }
    catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }

    t.join();
}

// 3. packaged task propagation
void show_packaged_task_propagation() {
    
    //An std::packaged_task has an associated std::future object that will hold the exception(or the result of the task)
    // You can retrieve that future by calling the get_future() member function of std::packaged_task.

    //This means that it is enough to throw an exception inside the function associated 
    // with the packaged task in order for that exception to be caught by the task's future

    std::packaged_task<int()> task{ cpp4::thread_throw_excetion };

    std::future<int> f = task.get_future();

    std::thread t{ std::move(task) };

    try{
        // process exception from get()
        int res = f.get();
    }
    catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
    t.join();
}

int main() {
    
    // propagation
    show_exception_propagation();
    show_multithreaded_propagation();
    show_packaged_task_propagation();

    // system_error

    return 0;
}
