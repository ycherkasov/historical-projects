#include <iostream>
#include <stdlib.h>
#include <sys/resource.h>
#include <string.h>
#include <errno.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 7 Stievens, Rago
 * main(), exit(), _exit(), environ(), atexit()
 *
 * 1. Нормально завершить программу можно по return, exit() (POSIX),
 *    _exit()/_Exit()(ANSI C), завершению всех потоков
 * 2. Аварийно можно завершить по abort() или сигналу
 * 3. Управление переменными окружения - putenv()/setenv()/unsetenv()
 * 4. Просмотреть переменные окружения - char** environ
 * 5. Посмотреть системные ограничения, наложенные на процесс -
 *    setrlimit()/getrlimit()
 */

/** @brief Show limit value or infinity */
void print_limit_value(rlim_t l) {
    if (l == RLIM64_INFINITY) {
        cout << "Inf" << endl;
    }
    else {
        cout << l << endl;
    }
}

/** @brief Get limit values */
void check_limit(int limit) {
    struct rlimit lim = {};
    if (getrlimit(limit, &lim) < 0) {
        cerr << strerror(errno) << endl;
    }
    cout << "Current = ";
    print_limit_value(lim.rlim_cur);
    cout << "Max = ";
    print_limit_value(lim.rlim_max);

}

void print(const char* name) {
    cout << name << " = " << endl;
}

#define PRINT_LIMITATION(name) print(#name); check_limit(name);

/** @brief Exit handlers */
static void my_exit1() {
    cout << "Exit handler 1" << endl;
}

static void my_exit2() {
    cout << "Exit handler 2" << endl;
}

int main(int argc, char* argv[]) {

    // atexit() lets to register up to 32 exit handlers
    if (atexit(my_exit1) != 0) {
        cerr << "Unable to register" << endl;
    }

    // register handler my_exit2() twice to perform it twice
    if (atexit(my_exit2) != 0) {
        cerr << "Unable to register" << endl;
    }
    if (atexit(my_exit2) != 0) {
        cerr << "Unable to register" << endl;
    }

    // env lookup
    cout << "Environment lookup" << endl;
    int i = 0;
    while (environ[i] != 0) {
        cout << environ[i] << endl;
        ++i;
    }
    // максимальная доступная память
    PRINT_LIMITATION(RLIMIT_AS);

    // максимальный размер дампа памяти
    PRINT_LIMITATION(RLIMIT_CORE);

    // максимальное кол-во процессорного времени сек
    PRINT_LIMITATION(RLIMIT_CPU);

    // максимальный размер сегмента данных
    PRINT_LIMITATION(RLIMIT_DATA);

    // максимальный размер файла
    PRINT_LIMITATION(RLIMIT_FSIZE);

    // максимальное кол-во блокировок
    PRINT_LIMITATION(RLIMIT_LOCKS);

    // максимальный размер блокируемой памяти memlock()
    PRINT_LIMITATION(RLIMIT_MEMLOCK);

    // максимальное кол-во открытых файлов
    PRINT_LIMITATION(RLIMIT_NOFILE);

    // максимальное кол-во дочерних процессов
    PRINT_LIMITATION(RLIMIT_NPROC);

    // максимальное кол-во страниц вирт. памяти
    PRINT_LIMITATION(RLIMIT_RSS);

    // максимальный размер стека
    PRINT_LIMITATION(RLIMIT_STACK);

    // finished badly
    // abort()

    // finished ok (POSIX)
    // closes all streams
    exit(0);

    // finished ok (ANSI C)
    _exit(0); // _Exit();

    // finished ok
    return 0;
}