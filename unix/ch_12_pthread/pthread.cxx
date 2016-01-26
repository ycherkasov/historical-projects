#include <pthread.h>
#include <iostream>
#include <stdexcept>
#include <limits.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 12 Stievens, Rago. 
 *
 */

/** @brief Show system thread limitations */
void thread_limits(){
    // max tries to call thread data destructor (see thread_keys)
    cout << "PTHREAD_DESTRUCTOR_ITERATIONS=" << sysconf(PTHREAD_DESTRUCTOR_ITERATIONS) << endl;

    // max shared thread keys
    cout << "PTHREAD_KEYS_MAX=" << sysconf(PTHREAD_KEYS_MAX) << endl;

    // min thread stack
    cout << "PTHREAD_STACK_MIN=" << sysconf(PTHREAD_STACK_MIN) << endl;
}


/** @brief Show thread attributes while starting thread */
void thread_attr(){

    /* Thread attributes list:
     * 1. Detachable
     * 2. Guard size (buffer at the top of stack)
     * 3. Stack start address and size
     *
     * Additional attributes:
     * 1. Thread multiplexing (user-space-threads mapping to kernel threads)
     * 2. Thread cancellation availability
     */

}

int main(int argc, char* argv[]) {

    try {

#if 1
        // thread limits
        thread_limits();
#endif

    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}
