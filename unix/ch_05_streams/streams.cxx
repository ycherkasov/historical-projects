#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

/*
 * Chapter 5 Stievens, Rago
 * POSIX Streams
 *
 * 1. Буфферизация м.б. полной, построчной и отсутствовать.
 * 2. Для создания временных фалов использовать tmpfile()/mkstemp().
 * 3. tmpnam/tempnam - non-thread-safe, deprecated
 * 4. Дескриптор из FILE можно получить функцией int fileno(FILE*)
 */

/** @brief Show bufferistion type */
void show_buffer_type(FILE* stream) {

    if (stream->_IO_file_flags & _IO_UNBUFFERED) {
        cout << "Unbuffered" << endl;
    }
    else if (stream->_IO_file_flags & _IO_LINE_BUF) {
        cout << "Line buffering" << endl;
    }
    else {
        cout << "Full buffering" << endl;
    }

    // show buffer size
    size_t bufsiz = stream->_IO_buf_end - stream->_IO_buf_base;
    cout << "Buffer size = " << bufsiz << endl;
}

/** @brief Show temporary files 1 */
void show_tempfile1(){

    // 1. Generate name only
    string tmp_filename( tmpnam(NULL) );
    cout << "Tmp file name: " << tmp_filename <<endl;
    // warning: the use of `tmpnam' is dangerous, better use `mkstemp'

    // 2. Generate tmp file
    FILE* fp = tmpfile();
    cout << "Tmp file created: " << fp->_fileno <<endl;

    // 3. Generate file in directory . with pefix tmp
    string tmp_filename2( tempnam(".", "tmp") );
    cout << "Tmp file name: " << tmp_filename <<endl;
    // warning: the use of `tempnam' is dangerous, better use `mkstemp'

    // 3. Generate file with suffix (thread-safe version, preferred!)
    char templ[] = "tmpfileXXXXXX";
    int des = mkstemp(templ);
    if(des == -1){
        cerr << strerror(errno) << endl;
    }
    cout << "Tmp file handle=" << des << ", file name: " << templ <<endl;
}


/** @brief Get stream descriptor by file name or standard stream */
/** @param filename file or standard stream (stdin, stdout, stderr) */
/** @return Stream descriptor */
FILE* get_stream(const string& filename) {
    std::map<std::string, FILE*> standard_streams;
    standard_streams["stdin"] = stdin;
    standard_streams["stdout"] = stdout;
    standard_streams["stderr"] = stderr;
    std::map<std::string, FILE*>::iterator it = standard_streams.begin();

    if ((it = standard_streams.find(filename)) == standard_streams.end()) {
        FILE* fp = 0;
        if (NULL == (fp = fopen(filename.c_str(), "r"))) {
            std::stringstream s;
            s << "Error open " << filename;
            throw std::runtime_error(s.str().c_str());
        }
        return fp;
    }
    return (*it).second;

}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: program <filename>" << endl;
    }


    std::string filename(argv[1]);
    FILE* fp = get_stream(filename);

    cout << "Stream name = " << filename << endl;
    show_buffer_type(fp);

    show_tempfile1();

    return 0;
}
