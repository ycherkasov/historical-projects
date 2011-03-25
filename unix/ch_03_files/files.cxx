#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string.h>
#include <sys/ioctl.h>

/* 1. Use open() or create() with flags to create files.
 * 2. Use lseek() to change file position.
 * 3. read()/write() pair is not thread-safe with lseek(). Use pread()/pwrite()
 * 4. Use dup()/dup2() to duplicate handle
 * 5. Use sync()/fsync() to flush data on disk
 * 6. Use fnctl() to edit opened file properties
 * 7. Use ioctl() for low-level access
 */

// show main class operations with this class
class posix_file_t {
public:

    posix_file_t(int flags) : _flags(flags) { }

    posix_file_t(const char* name, int flags) : _flags(flags) {
        // create file if not exist (analog is creat() function)
        _handle = ::open(name, flags | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    }

    ~posix_file_t() {
        ::close(_handle);
    }

    // open file (close existing)

    void open(const char* name, int flags) {
        if (_handle) {
            ::close(_handle);
        }
        _handle = ::open(name, flags);
    }

    /** @brief this version of lseek returns current file position */

    /*  @param where  can be :
     SEEK_SET - from beginning of file
     SEEK_CUR -from current pos
     SEEK_END - from end of file */
    off_t lseek(off_t offset, int where) {
        return ::lseek(_handle, offset, where);
    }

    /**@brief this version of lseek returns current file position */
    off_t current_position() {
        return ::lseek(_handle, 0, SEEK_CUR);
    }

    /** @brief read 'bytes' to vector */
    std::vector<char> read(size_t bytes) {
        std::vector<char> buf(bytes, 0);
        ssize_t read = ::read(_handle, &buf[0], bytes);
        if (read != bytes) {
            std::cerr << "Warning: read less then " << bytes << std::endl;
        }
        return buf;
    }

    /** @brief write vector to file */
    void write(const std::vector<char>& buf) {
        ssize_t written = ::write(_handle, &buf[0], buf.size());
        if (written != buf.size()) {
            std::cerr << "Error: written less than less then "
                    << buf.size() << std::endl;
        }
    }

    /** @brief thread-safe version of 'read' ( equal to lseek()+read() ) */
    std::vector<char> atomic_read(off_t offset, size_t bytes) {
        std::vector<char> buf(bytes, 0);
        ssize_t read = ::pread(_handle, &buf[0], bytes, offset);
        if (read != bytes) {
            std::cerr << "Warning: read less then " << bytes << std::endl;
        }
        return buf;
    }

    /** @brief thread-safe version of 'write'  ( equal to lseek()+write() ) */
    void atomic_write(off_t offset, const std::vector<char>& buf) {
        ssize_t written = ::pwrite(_handle, &buf[0], buf.size(), offset);
        if (written != buf.size()) {
            std::cerr << "Error: written less than less then "
                    << buf.size() << std::endl;
        }
    }

    /** @brief duplicates file handle */
    int duplicate_handle() {
        int new_handle = ::dup(_handle);
        if (new_handle == -1)
            throw std::runtime_error("Error duplicate handle");
        return new_handle;
    }

    /** @brief duplicates file handle with concrete value */
    int duplicate_handle(int new_handle) {
        int new_handle2 = ::dup2(_handle, new_handle);
        if (new_handle2 == -1)
            throw std::runtime_error(strerror(errno));
        return new_handle2;
    }

    /** @brief write imidiately */
    void sync() {
        if ((-1) == ::fsync(_handle)) {
            throw std::runtime_error(strerror(errno));
        }
    }

    /** @brief write imidiately data only (without attributes)*/
    void datasync() {
        if ((-1) == ::fdatasync(_handle)) {
            throw std::runtime_error(strerror(errno));
        }
    }

    /** @brief Changes file attributes */
    /** @param command
     * F_DUPFD - duplicate handle
     * F_GETFD | F_SETFD - get/set descriptor flags
     * F_GETFL | F_SETFL - get/set descriptor state flags */
    void set_attr(int command, int arg) {
        int ret = ::fcntl(_handle, command, arg);
        if ((-1) == ret) {
            throw std::runtime_error(strerror(errno));
        }
    }

    /** @brief Low-level file control */
    /** @param request  Low-level operation */
    /** @param arg  pointer to value or structure */
    template <typename Arg>
    void ioctl(unsigned long request, Arg arg){
        int ret = ::ioctl(_handle, request, arg);
        if ((-1) == ret) {
            throw std::runtime_error(strerror(errno));
        }
    }

private:
    int _handle;

    // flags : O_RDONLY | O_WRONLY | O_RDWR
    int _flags;
};

int main(int argc, char* argv[]) {

    try {
        posix_file_t myfile("myfile.txt", O_RDWR);
        off_t offset = myfile.current_position();
        std::cout << "offset: " << offset << std::endl;

        offset = myfile.lseek(10, SEEK_SET);
        std::cout << "offset: " << offset << std::endl;

        // create a 'hole' - write 5 characters in offset 10
        std::vector<char> v(5, 'a');
        myfile.write(v);

        // currently filesize is 15, content is 'aaaaa'

        // you can use /dev/fd directory to view or use all opened file handles

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
