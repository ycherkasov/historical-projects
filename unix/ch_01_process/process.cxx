#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <iterator>

/*
 Семейство функций  exec...  загружает  и  запускает
другие    программы,   известные   как   "дочерние"
процессы.  Если вызов функции  exec...  завершается
успешно,   "дочерний"   процесс   накладывается  на
"родительский"   процесс;   причем   должно    быть
достаточно   памяти   для   загрузки  и  выполнения
"дочернего" процесса.

Суффиксы l, v, p и e, добавляемые к имени семейства
exec...   обозначают,   что  данная  функция  будет
работать с некоторыми особенностями:
p - определяет, что функция будет искать "дочернюю"
программу     в    директориях,    определяемых
переменной среды PATH. Без суффикса p поиск
будет  производиться только в рабочем каталоге.

l - показывает, что адресные указатели (arg0, arg1,
..., argn) передаются, как отдельные аргументы.
Обычно суффикс l употребляется, когда число пере-
даваемых аргументов заранее вам известно.

v - показывает,  что  адресные  указатели  (arg[0],
arg[1],...arg[n])    передаются,   как   массив
указателей.  Обычно,  суффикс  v  используется,
когда передаeтся переменное число аргументов

e - показывает, что "дочернему" процессу может быть
передан  аргумент   envp,   который   позволяет
выбирать   среду   "дочернего"   процесса.  Без
суффикса e "дочерний" процесс унаследует  среду
"родительского" процесса.
 *  */

// define function-signal handler
static void sig_interrupt(int){
    std::cout << "interrupted\n%% ";
}

// analog of WinAPI CreateProcess
void create_process(const char* procname){
    pid_t pid = 0;
    int status = 0;
    
    if( (pid = fork()) < 0 ){
	throw std::runtime_error("fork() error");
    }
    else if( pid == 0 ){
	execlp( procname, procname, (char*)0 );
	
	// we should not be here if OK
	std::cerr << "Unable to execute " << procname << std::endl;
	exit(127);
    }
    
    // wait for child start
    if( pid = waitpid(pid, &status, 0) < 0 ){
	std::cerr << "waitpid() error" << std::endl;
    }
}

// class represent simple unix shell with promt string '%%'
struct simple_shell{
    
    simple_shell(){
	std::cout << "%% ";
    }
    
    void start(){
	
	char buf[255];
	while( NULL != fgets(buf, 255, stdin)  ){

	    // set endline as '0'
	    size_t s = strlen(buf) - 1;
	    if (buf[s] == '\n'){
		buf[s] = 0;
	    }

	    create_process(buf);
	    std::cout << "%% ";
	}
	
	
    }
};

int main(int ac, char* av[]) {

    // try to handle signal Ctrl-C
    if( SIG_ERR == signal(SIGINT, sig_interrupt) ){
	std::cout << "signal() function error" << std::endl;;
    }

    simple_shell s;
    s.start();

    return 0;
}

