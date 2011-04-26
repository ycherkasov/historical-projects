#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 10 Stievens, Rago. Signals.
 *
 * 1. Signals are async interruptions (like SEH in windows)
 * 2. Include <signals.h> only (other signals header included there)
 * 3. There is no signal 0
 * 4. Signal actions are default, ignore, user-defined (except SIGKILL & SIGSTOP)
 * 5. signal() is deprecated, use sigaction() instead.
 * 6. pause() waits for any signal
 * 7. Signal can be cause of restart blocking system calls (select, read, so on)
 * 8. Don't use non-reenterant and thread-unsafe functions in signal handler.
 *    Behavour is undefined in this case.
 * 9. You should re-set signal handler entering in signal handler function,
 *    because of while processing the next signal can be lost
 * 10.Use kill() to send signal other process, raise() to send it to self process.
 * 11.Don't use sigprocmask() in muttithreaded application
 * 12.Use strsignal() ir sys_siglist[] to get string representation of signal.
 * 13. 
 */

typedef void sig_handler_t(int);

void reset_user_handlers();

static void user_signal_handler(int signo) {

    // re-set handler to prevent signal loss in multi-threaded environment
    reset_user_handlers();
    cout << "Signal " << signo << " == " << sys_siglist[signo]
            << ";" << strsignal(signo) << endl;
    exit(0);
}

void reset_user_handlers() {
    // send kill -10, kill -12 for SIGUSR
    if (signal(SIGUSR1, user_signal_handler) == SIG_ERR) {
        throw std::runtime_error(strerror(errno));
    }
    if (signal(SIGUSR2, user_signal_handler) == SIG_ERR) {
        throw std::runtime_error(strerror(errno));
    }
}

/** @brief signal() is deprecated function: catch signals SIGUSR1,2 */
void show_signal() {

    reset_user_handlers();
    // wait for any signal
    for (;;) {
        pause();
    }
}

/** @brief kill 0 used to check PID existance  */
void check_process_exist(pid_t p) {

    if (::kill(p, 0) == -1) {
        cout << "Process doesn't exist" << endl;
    }
    else {
        cout << "Process exist" << endl;
    }
}

/** @brief this is listetning operation as example of blocking op  */
void long_blocking_operation() {
    struct sockaddr_in local = {};
    local.sin_family = AF_INET;
    local.sin_port = htons(7500);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    // server: socket-bind-listen pattern
    // TCP store information about packet order (TCP window)
    // if packets 1-3 have been received, TCP window is 4-12

    // open inet socket for TCP connection (0 for TCP)
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        throw std::runtime_error(strerror(errno));
    }

    // bind to just opened socket for listening
    int rc = bind(s, (struct sockaddr *) &local, sizeof ( local));
    if (rc < 0) {
        throw std::runtime_error(strerror(errno));
    }

    // start listening (5 is maximum waited connections)
    rc = listen(s, 5);
    if (rc) {
        throw std::runtime_error(strerror(errno));
    }

    // accetp first connection from incoming queue
    // (params 2&3 are client information fields)
    int s1 = accept(s, NULL, NULL);
    if (s1 < 0) {
        throw std::runtime_error(strerror(errno));
    }

}

/** @brief Interrupt long operation in this handler  */
static void sig_alarm_empty(int signo){
    cout << "Wait interrupted" << endl;
    exit(0);
}

/** @brief Interrupt long operation using signal+alarm+pause  */
void interrupt_long_operation() {
    if( ::signal(SIGALRM, sig_alarm_empty) ){
        throw std::runtime_error(strerror(errno));
    }
    alarm(10);
    long_blocking_operation();
    alarm(0);
    exit(0);
 }

/** @brief Block signal SIGQUIT with sigprocmask() (single-thread only) */
void block_signal(){
    // blocked signal will wait in queue!

}

void show_sigaction(){}

// TODO : TELL_WAIT and so on (10.16)

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout << "Usage: program <filename>" << std::endl;
    }
    std::string pid(argv[1]);


    try {
        show_signal();
        //check_process_exist(boost::lexical_cast<pid_t > (pid.c_str()));
        //interrupt_long_operation();
    }
    catch(boost::bad_lexical_cast& e){
        cerr << e.what() << "(bad port name?)" << endl;
    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}
