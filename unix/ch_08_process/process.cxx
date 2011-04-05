#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <iostream>
#include <stdexcept>


using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 8 Stievens, Rago
 * Processes, fork, vfork, exec, wait, waitpid, system
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

int global_var = 6;

void show_fork() {

    cout << "Before first fork" << endl;
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
        cout << "Child: var = " << var << "global_var = " << global_var << endl;
    }
    else if (pid > 0) {
        // parent process
        // wait for child start
        if (pid = waitpid(pid, &status, 0) < 0) {
            std::cerr << "waitpid() error" << std::endl;
        }
        cout << "Parent: var = " << var << "global_var = " << global_var << endl;
    }


}

void show_fork_files() { }

void show_fork_streams() { }

void show_vfork() { }

void show_wait() { }

void show_waitpid() { }

void show_exit_codes() { }

void show_prevent_zombie() { }

void show_waitid() { }

void show_wait3() { }

void show_wait4() { }

void show_race_condition() { }

void show_exec() { }

void show_suid() { }

void show_system() { }

void show_acct() { }

void show_times() { }

int main(int argc, char* argv[]) {

    //show_process_info();
    show_fork();

    return 0;
}
