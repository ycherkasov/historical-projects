#include <string.h>
#include <errno.h>
#include <iostream>
#include <stdexcept>


using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 9 Stievens, Rago - Process groups
 *
 * 1. Processes can be joined into group. It used to use for cmd1 | cmd2 | cmd3
 *    processing for example.
 * 2. Process group can be get by getpgrp()/getpgid(pid)
 * 3. Process group can be changed by setpgid(pid, pidgroup)
 *
 * TODO:
 * 1. Разобраться с группами процессов и сессиями.
 */

/** @brief get process group of current process and by PID */
void show_procgroup() {
    cout << "Process group: " << getpgrp() << endl;
    cout << "Parent process group: " << getpgid(getppid()) << endl;
}

/** @brief try to join other process group */
void show_join() {
    pid_t p = getpid();
    pid_t pg = getpgid(getppid());
    cout << "Process " << p << " tries to joins to group " << pg << endl;
    if (-1 == setpgid(p, pg)) {
        throw std::runtime_error(strerror(errno));
    }

}

/** @brief try to create new session and process group and become a leader */
void become_a_leader(){
    pid_t p = 0;
    if (-1 == (p = setsid())) {
        throw std::runtime_error(strerror(errno));
    }
    show_procgroup();
}
int main(int argc, char* argv[]) {

    try {
        show_procgroup();
        show_join();
        show_procgroup();
        // doesn't permitted!
        //become_a_leader();
    }
    catch(const std::exception& e){
        cerr << e.what() << endl;
    }
    return 0;
}
