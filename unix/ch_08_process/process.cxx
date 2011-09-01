#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/acct.h>
#include <sys/times.h>
#include <iostream>
#include <stdexcept>


using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 8 Stievens, Rago
 * Processes, fork, vfork, exec, wait, waitpid, system
 *
 * 1. Get PID/UID/GID: getpid/getppid/getuid/getgid/geteuid/getegid
 * 2. fork() inherits opened file handles, streams (output twice!), address space
 * 3. vfork() guarantees child process start before parent process, doesn't copy
 *    parent address space, and it is for usage with exec() only
 * 4. Child send SIGCHLD when finishes
 * 5. wait() wait for the first finished child process
 * 6. waitpid() wait for concrete child PID, and can be unblocked (just ask if child finished)
 * 7. Macroses can be used to analyze child exit code (exit() or abort() or signal)
 * 8. fork() can be performed twice to prevent zombie process
 *    (after 2-nd fork init becomes his parent )
 * 9. waitid() can be used to analyse signal which finished child process
 *    and for child process task management
 * 10.wait3()/wait4() alse returns system information about child process.
 *    wait3() is deprecated.
 * 11.UID/GID can be changed by setuid/setgid/seteuid/setegid (must be root!)
 * 12.System starts processes like bash (can not analyze pid and return code!)
 * 13.init must be a parent of any process. Process without parent is a zombie.
 * 14.fork() doesn't copies all address space imidiately.
 *    It performs copy-on-write instead.
 * 15.Flushing stream buffers prevent output duplicate after fork()
 * 16.fork() uses in 2 cases: 1-exec() 2-multiprocess server (like multithread)
 * 17.exit() flushes all buffers before exit. _exit() doesn't
 * 18.system(NULL) used to check bash existance in system
 * 19.Use getlogin() just to get logged user name
 * 20.Use times() to get process and its child user and system time
 *
 * TODO :
 * 1. Как работать с acct
 * 2. Неправильно коапилируется пример с шаблонами
 */

/** @brief show process and user ID */
void show_process_info() {
    cout << "PID = " << getpid() << endl;
    cout << "Parent PID = " << getppid() << endl;
    cout << "Real UID = " << getuid() << endl;
    cout << "Real GID = " << getgid() << endl;
    cout << "Effective UID = " << geteuid() << endl;
    cout << "Effective GID = " << getegid() << endl;
}

// just for example
int global_var = 6;

/** @brief show process fork and output streams inheritance */
void show_fork() {

    cout << "Buffer flushed " << endl;
    // Если мы не сбросим буфер явно (/n или cout)
    // сообщение продублируется в двух процессах,
    // т.к. буферизированные потоки также будут наследованы
    cout << "Buffered ";


    // just for example
    int var = 88;

    pid_t pid = 0;
    if ((pid = fork()) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid == 0) {
        // child process
        var++;
        global_var++;
        cout << "Child: var = " << var << " global_var = " << global_var << endl;
        show_process_info();
    }
    else if (pid > 0) {
        // parent process
        // wait for child start (without it child-parent continue order is undefined)
        int status = 0;
        if (pid = waitpid(pid, &status, 0) < 0) {
            std::cerr << "waitpid() error" << std::endl;
        }
        cout << "Parent: var = " << var << " global_var = " << global_var << endl;
        show_process_info();
    }
}

/** @brief show fork and file handles inheritance */
void show_fork_files() {

    // fork inherits all opened file handles, uid, gid, suid and address space
    int handle = ::open("test.txt", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    pid_t pid = 0;
    if ((pid = fork()) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid == 0) {
        // child process
        cout << "Child: File handle " << handle << endl;
        cout << "Child: File position " << ::lseek(handle, 0, SEEK_CUR) << endl;
        show_process_info();
        ::close(handle);
    }
    else if (pid > 0) {
        // parent process
        // wait for child start (without it child-parent continue order is undefined)
        int status = 0;
        if (pid = waitpid(pid, &status, 0) < 0) {
            std::cerr << "waitpid() error" << std::endl;
        }
        cout << "Parent: File handle " << handle << endl;
        cout << "Parent: File position " << ::lseek(handle, 0, SEEK_CUR) << endl;
        show_process_info();
        ::close(handle);
    }
}

/** @brief show vfork and exec */
void show_vfork() {
    // 1. vfork guarantees child process start before parent process
    // 2. vfork doesn't copies parent address space
    // 3. vfork is for usage with exec() only

    pid_t pid = 0;
    if ((pid = vfork()) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid == 0) {
        // child process (exec() should be called here)
        cout << "Child " << endl;
        show_process_info();
        _exit(0);
        // do not call exec(), it flushes streams in both parent and child,
        // because its address space is shared
    }
    else if (pid > 0) {
        // parent process
        // after vfork always wait
        cout << "Parent " << endl;
        show_process_info();
    }

}

// Look for nessesary handles below:

struct child_handler {

    void operator()() {
        cout << "Child " << endl;
        show_process_info();
        // let the parent process wait a little
        sleep(5);
        _exit(0);
    }
};

struct parent_handler_wait {

    void operator()(pid_t p) {
        pid_t pid = 0;

        // child process return code
        int status = 0;
        cout << "Wait for the first finished child process... " << endl;
        // wait for the first finished child process
        if (pid = wait(&status) < 0) {
            throw std::runtime_error(strerror(errno));
        }
        cout << "Wait finished" << endl;
        cout << "Parent: " << endl;
        show_process_info();
    }
};

struct parent_handler_waitpid {

    void operator()(pid_t p) {
        pid_t pid = 0;
        // child process return code
        int status = 0;

        cout << "Wait for pid = " << p << endl;
        // wait for concrete child process
        if (pid = waitpid(p, &status, 0) < 0) {
            throw std::runtime_error(strerror(errno));
        }
        cout << "Wait finished" << endl;
    }
};

template <typename ParentHandler, typename ChildHandler>
void fork_handler() {
    pid_t pid = 0;
    if ((pid = fork()) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid == 0) {
        // child process (exec() should be called here)
        ChildHandler();
    }
    else if (pid > 0) {
        // wait for the first
        // after vfork always wait
        ParentHandler(pid);
    }
}

/** @brief show waiting for child process finishes */
void show_wait() {
    // Child process send SIGCHLD to a parent when finishes
    // it is possible to:
    // 1. Wait for child finishes
    // 2. Get return code if its already has been finished
    // 3. Get error code if child process does not exist
#if 0
    fork_handler<parent_handler_wait, child_handler > ();
#else
    pid_t pid = 0;
    if ((pid = fork()) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid == 0) {
        // child process (exec() should be called here)
        cout << "Child " << endl;
        show_process_info();
        // let the parent process wait a little
        sleep(5);
        _exit(0);

    }
    else if (pid > 0) {
        // wait for the first
        // after vfork always wait
        pid_t pid = 0;
        // child process return code
        int status = 0;
        cout << "Wait for the first finished child process... " << endl;
        // wait for the first finished child process
        if (pid = wait(&status) < 0) {
            throw std::runtime_error(strerror(errno));
        }
        cout << "Wait finished" << endl;
        cout << "Parent: " << endl;
        show_process_info();
    }
#endif
}

/** @brief show waiting for child process finishes */
void show_waitpid() {
    fork_handler<parent_handler_waitpid, child_handler > ();
}

/** @brief analyze child process return code */
void analyze_exit_code(int retcode) {
    if (WIFEXITED(retcode)) {
        cout << "Finished OK" << endl;
        return;
    }
    else if (WIFSIGNALED(retcode)) {
        cout << "Aborted by signal: " << WTERMSIG(retcode) << endl;
        if (WCOREDUMP(retcode)) {
            cout << "Coredump created" << endl;
        }
        return;
    }
    else if (WIFEXITED(retcode)) {
        cout << "Finished OK by signal: " << WSTOPSIG(retcode) << endl;
        return;
    }
    return;
}

/** @brief show child process ret code analyze */
void show_exit_codes() {
    pid_t pid = 0;
    if ((pid = fork()) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid == 0) {
        // child process (exec() should be called here)
        cout << "Child " << endl;
        show_process_info();
        // let the parent process wait a little
        sleep(1);

        // Child finish cases:

        // 1. finish ok
        _exit(0);

        // 2. abort by signal SIGABRT
        //abort();

        // 3. finish by signal SIGFPE (divide by 0)
        //int ret = 10/pid;

    }
    else if (pid > 0) {
        // wait for the first
        // after vfork always wait
        pid_t pid = 0;
        // child process return code
        int status = 0;
        cout << "Wait for the first finished child process... " << endl;
        // wait for the first finished child process
        if (pid = wait(&status) < 0) {
            throw std::runtime_error(strerror(errno));
        }
        cout << "Wait finished" << endl;
        analyze_exit_code(status);
    }
}

/** @brief show trick to prevent zombie */
void show_prevent_zombie() {

    pid_t pid = 0;

    // create first child
    if ((pid = fork()) < 0) {
        throw std::runtime_error(strerror(errno));
    }
        // First child, parent for second one
    else if (pid == 0) {

        // create second child
        if ((pid = fork()) < 0) {
            throw std::runtime_error(strerror(errno));
        }
            //Second child
        else if (pid == 0) {
            cout << "Second child (parent is init!): " << endl;
            show_process_info();
            sleep(2);
            exit(0);
        }
        else if (pid > 0) {
            cout << "First child: " << endl;
            show_process_info();
            exit(0);
        }

    }
    else if (pid > 0) {

        // wait for the finished first child process
        if (pid = waitpid(pid, NULL, 0) < 0) {
            throw std::runtime_error(strerror(errno));
        }
        cout << "Parent: " << endl;
        show_process_info();
        exit(0);
    }

}

/** @brief show waitid - more flexible function than waitpid */
void show_waitid() {
    pid_t pid = 0;
    if ((pid = fork()) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid == 0) {
        // child process (exec() should be called here)
        cout << "Child: " << endl;
        show_process_info();
    }
    else if (pid > 0) {
        int ret = 0;
        siginfo_t child_signal = {};

        cout << "Wait for the first finished child process... " << endl;

        // waitid is more flexible
        // (1 and 4 arguments are options to wait for specific processes)
        if (ret = waitid(P_PID, pid, &child_signal, WEXITED) == -1) {
            throw std::runtime_error(strerror(errno));
        }
        cout << "Wait finished" << endl;
    }
}

/** @brief show wait3, wait4 - provides information about child process resources */
void show_wait34() {

    pid_t pid = 0;
    if ((pid = fork()) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid == 0) {
        // child process (exec() should be called here)
        cout << "Child: " << endl;
        show_process_info();
        sleep(1);
        exit(0);
    }
    else if (pid > 0) {
        pid_t ret = 0;
        int status = 0;
        rusage res = {};

        // wait3 - deprecated
        cout << "Wait for the first finished child process... " << endl;
        if (ret = wait4(pid, &status, 0, &res) <= 0) {
            throw std::runtime_error(strerror(errno));
        }
        cout << "Wait finished, child process info: " << endl;
        cout << "User time: " << res.ru_utime.tv_sec << endl;
        cout << "System time: " << res.ru_stime.tv_sec << endl;
        cout << "Segment memory used: " << res.ru_ixrss << endl;
        cout << "Stack memory used: " << res.ru_isrss << endl;
        cout << "And so on... " << endl;
    }
}

// todo
void show_race_condition() { }

// todo
void show_exec() { }

/** @brief set new UID and GID (must be run as sudo) */
void show_suid() {

    show_process_info();
    cout << "Set new UID/GID" << endl;
    if(-1 == setuid(1001) ){
        throw std::runtime_error( strerror(errno) );
    }
    if(-1 == setgid(1001) ){
        throw std::runtime_error( strerror(errno) );
    }

    cout << "Set new effective UID/GID" << endl;
    if(-1 == seteuid(1001) ){
        throw std::runtime_error( strerror(errno) );
    }
    if(-1 == setegid(1001) ){
        throw std::runtime_error( strerror(errno) );
    }

    show_process_info();
}

/** @brief start as command string */
void show_system() {
    
    // check if bash exist
    if( system(NULL) ){
        cout << "Bash exist" << endl;
        system("ls");
    }
}

void show_times() {
    tms mytime = {};
    cout << "Common time: " << times(&mytime) << endl;
    cout << "System time: " << mytime.tms_stime << endl;
    cout << "User time: " << mytime.tms_utime << endl;
}

int main(int argc, char* argv[]) {

    try {
        //show_process_info();
        //show_fork();
        //show_fork_files();
        //show_vfork();
        //show_wait();
        //show_waitpid();
        //show_exit_codes();
        //show_prevent_zombie();
        //show_waitid();
        //show_wait34();
        //show_suid();
        //show_system();
        show_times();
    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
