#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// demonstrates non-buffered and buffered output in POSIX

struct output_copier {
    // using system calls directly

    static void nonbuffer_out() {

        int n = 0;
        const size_t bufsize = 4096;
        char buf[bufsize] = {};
        while ((n = read(STDIN_FILENO, buf, bufsize)) > 0) {

            if (n != write(STDOUT_FILENO, buf, n)) {
                std::cerr << "write error" << std::endl;
            }

            if (n < 0) {
                std::cerr << "read error" << std::endl;
            }
        }
    }

    // using wrappers under system calls with bufferisation
    // (instead of manual 'buf' usage in previous method)

    static void buffered_out() {

        int c = 0;
        while (EOF != (c = getc(stdin))) {

            if (EOF == putc(c, stdout)) {
                std::cerr << "write error" << std::endl;
            }

            if (ferror(stdin)) {
                std::cerr << "read error" << std::endl;
            }
        }

    }
};

void show_uid() {
    std::cout << "uid = " << getuid() << " gid = " << getgid() << std::endl;
}

void show_malloc() {
    // malloc() is wrapper under brk()/sbrk() system call
    void* a = malloc(10);
    free(a);
#if 0
    printf("Changing allocation with sbrk()\n");
    printf("Before sbrk() call: %lu bytes free\n", (unsigned long) coreleft());
    sbrk(1000);
    printf(" After sbrk() call: %lu bytes free\n", (unsigned long) coreleft());
#endif
}

int main(int argc, char* argv[]) {

    show_uid();
    show_malloc();
#if 1
    output_copier::buffered_out();
#else
    output_copier::nonbuffer_out();
#endif
    return 0;
}
