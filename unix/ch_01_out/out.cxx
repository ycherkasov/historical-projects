#include <stdio.h>
#include <iostream>

// demonstrates non-buffered and buffered output in POSIX

struct output_copier{

    // using system calls directly
    static void nonbuffer_out(){

	int n = 0;
	const size_t bufsize = 4096;
	char buf[bufsize] = {};
	while( (n = read(STDIN_FILENO, buf, bufsize)) > 0 ){

	    if( n != write(STDOUT_FILENO, buf, n) ){
		std::cerr << "write error" << std::endl;
	    }

	    if (n < 0){
		std::cerr << "read error" << std::endl;
	    }
	}
    }

    // using wrappers under system calls with bufferisation
    static void buffered_out(){

	int c = 0;
	while( EOF != (c = getc(stdin)) ){

	    if( EOF == putc(c, stdout) ){
		std::cerr << "write error" << std::endl;
	    }

	    if ( ferror(stdin) ){
		std::cerr << "read error" << std::endl;
	    }
	}

    }
};

int main(int argc, char* argv[]){

#if 1
    output_copier::buffered_out();
#else
    output_copier::nonbuffer_out();
#endif
    return 0;
}
