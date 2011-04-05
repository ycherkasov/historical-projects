#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <stdexcept>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

/* 1. Use lstat() instead of stat() to see symbolic links
 * 2. Use chmod()/fchmod() - bash analogs
 * 3. Use S_ISXXX macro to define file type
 * 4. Use S_ISUID/S_ISGID and access() funaction to get real uid/gid
 * 5. Use chown/fchown/lchown as same as bash commands
 * 6. Use truancete/ftruncate to cut file
 * 7. Use link()/unlink() for hard links
 * 8. Use symlinc() to create symbolic links (and readlink() to open it)
 * 9. Use utime() to change last access and modification time
 * 10. Use mkdir()/rmdir() as same as bash
 * 11. Use chdir()/fchdir() as bash 'cd', use getcwd() as bash pwd
 *
 */

// Chapter 4. Stievens
// Filesystem, stat, access, umask ...

class file_info {
public:

    /** @brief file info by handle*/
    file_info(int handle) {
        int ret = ::fstat(handle, &_fileinfo);
        if ((-1) == ret) {
            throw std::runtime_error(strerror(errno));
        }
    }

    /** @brief file info by filename */
    file_info(const char* filename) {

        // use lstat() instead of stat() to see symbolic links
        int ret = ::lstat(filename, &_fileinfo);
        if ((-1) == ret) {
            throw std::runtime_error(strerror(errno));
        }
    }

    /** @brief Print file type using macros */
    std::string get_type(mode_t m){
        if(S_ISREG(m)) return std::string("Regular file");
        if(S_ISDIR(m)) return std::string("Directory");
        if(S_ISCHR(m)) return std::string("Character device");
        if(S_ISBLK(m)) return std::string("Block device");
        if(S_ISFIFO(m)) return std::string("Pipe");
        if(S_ISLNK(m)) return std::string("Symbolis link");
        if(S_ISSOCK(m)) return std::string("Socket");
        return std::string("Unknown type");
    }

    /** @brief Print if effective UID/GID is different then real */
    std::string show_effective_uid(mode_t m){
        if(S_ISUID & m) return std::string("Effective UID");
        if(S_ISGID & m) return std::string("Effective GUID");
        return std::string("UID same as real");
    }

    /** @brief Print all info */
    void print_info(){

        // show file type
        std::cout << "File mode: " << get_type(_fileinfo.st_mode)  << std::endl;

        // show if effective uid not as same as real
        std::cout << "Effective UID: " << show_effective_uid(_fileinfo.st_mode)  << std::endl;
        std::cout << "Index node: " << _fileinfo.st_ino << std::endl;
        
        // show device information as major/minor device format
        std::cout << "Device num: " << major(_fileinfo.st_dev)
                << "/" << minor(_fileinfo.st_dev) << std::endl;
        
        std::cout << "Link number: " << _fileinfo.st_nlink << std::endl;
        std::cout << "User id: " << _fileinfo.st_uid << std::endl;
        std::cout << "Group id: " << _fileinfo.st_gid << std::endl;
        std::cout << "Size: " << _fileinfo.st_size << std::endl;
        std::cout << "Last access: " << _fileinfo.st_atim.tv_sec << std::endl;
        std::cout << "Last modified: " << _fileinfo.st_mtim.tv_sec << std::endl;
        std::cout << "Disk blocks: " << _fileinfo.st_blocks << std::endl;
    }

    /** @brief Check permissions based on real UID/GID */
    /** @param R_OK - read, W_OK - write, X_OK - execute, F_OK - file exist */
    bool access(const char* name, int mode){
        if( 0 == ::access(name, mode)){
            return true;
        }
        return false;
    }

private:
    /** @brief File information */
    struct stat _fileinfo;
};

int main(int argc, char* argv[]) {
    if(argc != 2){
        std::cout << "Usage: program <filename>" << std::endl;
    }

    std::string filename(argv[1]);

    try{
        file_info f(filename.c_str());
        f.print_info();

        // set mask of all created files 644
        mode_t new_mask = S_IRGRP | S_IWGRP | S_IROTH | S_IROTH;
        mode_t old_mask = ::umask(new_mask);
        std::cout << "Old mask = " << old_mask << std::endl;
        std::cout << "New mask = " << new_mask << std::endl;

        // chmod()/fchmod() - bash analogs

    }
    catch(const std::exception& e){
        std::cout << e.what() << std::endl;
    }
    return 0;
}
