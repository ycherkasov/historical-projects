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
 * 13.You can check current signal handler via return value of signal() function.
 * 14.pid > 0 - send to specific process
 * 15.pid == 0 - broadcast
 * 16.pid < 0 - send to all processes in group group_id = |pid|
 * 17.pid == -1 - send to all processes except system (init and so on)
 * 18.Use sigset_t/sigemptyset()/sigfillset/sigaddset/sigdelset
 *    to work with signal groups (block signal handling, wait for signals etc.)
 * 19.Use sigprocmask() to set blocked signals
 * 20.Use sigpending() to get blocked signals mask
 * 21.Use sigpending() ??
 * 22.Use strsignal() to get signal name string, use psignal()
 *    to get signal name with custom message.
 */

// TODO : what is sigpending() ?

typedef void sig_handler_t(int);

void reset_user_handlers();

void show_signal_name(int signo){
    cout << "Signal " << signo << " == " << sys_siglist[signo]
            << ";" << strsignal(signo) << endl;

}


static void show_signal_handler(int signo) {
    // re-set handler to prevent signal loss in multi-threaded environment
    reset_user_handlers();
    show_signal_name(signo);
    exit(0);
}

void reset_user_handlers() {
    // note: signal() is deprecated, use sigaction instead
    // send kill -10, kill -12 for SIGUSR
    if (signal(SIGUSR1, show_signal_handler) == SIG_ERR) {
        throw std::runtime_error(strerror(errno));
    }
    if (signal(SIGUSR2, show_signal_handler) == SIG_ERR) {
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

void check_signal_handler(){

    // set signal ignore if it haven't done yet
    if (signal(SIGINT, SIG_IGN) != SIG_IGN)
        signal(SIGINT, SIG_IGN);
    if (signal(SIGQUIT, SIG_IGN) != SIG_IGN)
        signal(SIGQUIT, SIG_IGN);

}

/** @brief Show signal set usage */
void signal_set(){
    
    // signals set
    sigset_t signals_set1 = {};
    sigset_t signals_set2 = {};

    // init signals set with one of these functions:

    cout << "Init signals set" << endl;
    // 1.init signals set as empty
    ::sigemptyset(&signals_set1);

    // 2.init signals set with all signals
    ::sigfillset(&signals_set2);

    // add signals to set
    cout << "Add signals: SIGINT = " << SIGINT << " SIGUSR1 = " << SIGUSR1 << endl;
    ::sigaddset(&signals_set1, SIGINT);
    ::sigaddset(&signals_set1, SIGUSR1);

    // delete signals from set
    cout << "Delete signals: SIGCHLD = " << SIGCHLD << " SIGINT = " << SIGINT << endl;
    ::sigdelset(&signals_set2, SIGCHLD);
    ::sigdelset(&signals_set2, SIGINT);

    // check if the signal exist, return 1 if True, 0 if False, -1 if error
    int ret = ::sigismember(&signals_set1, SIGINT);
    cout << "SIGINT exist: " << ret << endl;

    // check if the signal set is empty
    ret = ::sigisemptyset(&signals_set1);
    cout << "signals_set1 is empty: " << ret << endl;


    // get current process mask
    sigset_t current_set = {};
    ::sigprocmask(0, NULL, &current_set);

    if( ::sigismember(&current_set, SIGUSR1) ){
        show_signal_name(SIGUSR1);
    }


}

/** @brief Do someting with all signals */
void all_signals(sig_handler_t handler){
    for(int i = 1 ; i <= SIGUNUSED ; i++){
        handler(i);
    }
}

/** @brief Enumarate all signal names */
void show_all_signal_names(){
    all_signals(show_signal_name);
}

/** @brief Check if signals is in sigset */

/** @brief Block signal SIGQUIT with sigprocmask() (single-thread only) */
void block_signal(){
    // sigprocmask can get current signal mask or set the new one
    // (except SIGKILL and SIGSTOP)
    cout << "My PID is " << getpid() << endl;

    // get current process mask
    
    // signal mask before blocking
    sigset_t old_set = {};

    // signal mask before blocking
    sigset_t new_set = {};

    // lock signal mask
    sigset_t pending_set = {};
    
    // wait all signals except this mask
    sigset_t waiting_set = {};

    ::sigemptyset(&old_set);
    ::sigemptyset(&new_set);
    ::sigemptyset(&pending_set);
    ::sigemptyset(&waiting_set);

    // pass only these signals
    ::sigaddset(&waiting_set, SIGUSR1);
    ::sigaddset(&waiting_set, SIGUSR2);

    // block SIGQUIT
    ::sigaddset(&new_set, SIGQUIT);
    ::sigprocmask(SIG_BLOCK, &new_set, &old_set);

    // check if signal SIGQUIT blocked
    ::sigpending(&pending_set);
    if( ::sigismember(&pending_set, SIGQUIT) ){
        show_signal_name(SIGQUIT);
        cout << " is blocked" << endl;
    }

    // blocked untill all signals except SIGUSR1,2
    ::sigsuspend(&waiting_set);

    // restore old signal mask
    ::sigprocmask(SIG_SETMASK, &old_set, NULL);
    show_signal_name(SIGQUIT);
    cout << " is unblocked" << endl;

    // unblocked SIGQUIT finishes process here
    sleep(5);
    exit(0);
}

void show_sigaction() {
    

    // select all existing signals
    sigset_t sigmask ;
    sigfillset(&sigmask) ;

    // set new signal action
    struct sigaction sa;
    sa.sa_handler = show_signal_name;
    // restart system calls after signal call
    sa.sa_flags = SA_RESTART;
    ::sigemptyset(&sa.sa_mask);

    // these signals will be locked while handler is working
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGTERM);
    sigaddset(&sa.sa_mask, SIGQUIT);

    // for every existing signal (except SIGSTOP and SIGKILL)
    for (int sig = 0;32 != sig;++sig) {
        if (1 == sigismember(&sigmask, sig)){
            // show signal name with custom message
            ::psignal(sig, "Handling signal");
            sigaction(sig, &sa, NULL);
        }
            
    }
    sleep(10);
}


// TODO : TELL_WAIT and so on (10.16)


//#define PID_NEEDED
int main(int argc, char* argv[]) {

#ifdef PID_NEEDED
    if (argc != 2) {
        std::cout << "Usage: program <filename>" << std::endl;
    }
    std::string pid(argv[1]);
#endif

    try {
        //show_signal();
#ifdef PID_NEEDED
        //check_process_exist(boost::lexical_cast<pid_t > (pid.c_str()));
#endif
        //interrupt_long_operation();
        //signal_set();
        //show_all_signal_names();
        //block_signal();
        show_sigaction();
    }
    catch(boost::bad_lexical_cast& e){
        cerr << e.what() << "(bad port name?)" << endl;
    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}
