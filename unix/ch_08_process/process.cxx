#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 8 Stievens, Rago
 * Processes, fork, vfork, exec, wait, waitpid, system
 */

/** @brief show process and user ID */
void show_process_info(){
    cout << "PID = " << getpid() << endl;
    cout << "Parent PID = " << getppid() << endl;
    cout << "Real UID = " << getuid() << endl;
    cout << "Real GID = " << getgid() << endl;
    cout << "Effective UID = " << geteuid() << endl;
    cout << "Effective GID = " << getegid() << endl;

}

void show_fork(){}

void show_fork_files(){}

void show_fork_streams(){}

void show_vfork(){}

void show_wait(){}

void show_waitpid(){}

void show_exit_codes(){}

void show_prevent_zombie(){}

void show_waitid(){}

void show_wait3(){}

void show_wait4(){}

void show_race_condition(){}

void show_exec(){}

void show_suid(){}

void show_system(){}

void show_acct(){}

void show_times(){}

int main(int argc, char* argv[]) {

    show_process_info();

    return 0;
}
