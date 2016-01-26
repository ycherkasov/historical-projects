#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>

// From Satter 2. Ch.04 Optimizing Bh.05 Catches

// Ch 3 Task 24 const optimizes some values if compiler make sure it is not copied on the stack

// Ch 3 Task 25 Inline could optimize on the 
// * compiling
// * linking (post-compile analysis)
// * executing (multiple implementations, own byte-code etc.)

// In most cases inline instruction is ignored 
// (it is declined by cycles, recursion, dereference, virtual etc.)

// Ch 3 Task 26 Extreme data packaging
// Class like bitset, but variable length
class bit_buffer{
public:
    typedef unsigned char byte;

    // Append num bits
    void append(byte* p, size_t num){
        for (size_t i = 0; i < num; ++i){
            _buf.push_back(*p & (1 << (bits - 1 - i%bits)));
            if ((i + 1) % bits == 0){
                ++p;
            }
        }
    }

    void get(size_t start_byte, size_t num, byte* dst) const {
        *dst = 0;
        for (size_t i = 0; i < num; ++i){
            *dst |= byte(_buf[start_byte + i]) << (bits - 1 - i%bits);
            if ((i + 1) % bits == 0){
                *++dst = 0;
            }
        }
    }

    size_t size() const { return _buf.size(); }

private:
    // store raw bits
    std::vector<bool> _buf; 

    const int bits = std::numeric_limits<byte>::digits;
};

void show_bit_buffer(){
    bit_buffer b;

    unsigned char arr[] = { 'a', 'b', 'c' };
    
    // append 20 bit
    b.append(arr, 20);

    size_t sz = b.size();

    unsigned char arr_dst[3] = {};
    b.get(0, 20, arr_dst);

    for (size_t i = 0; i < 3;++i){
        std::cout << arr_dst[i];
    }
    std::cout << std::endl;
}

// Ch 3 Task 28 Ignored keyword
// register and inline are ignored by most compilers.
// Compiler knows better when to optimize

// Ch 3 Task 29 Initialization vs function declare
void show_function_declare(){

    using namespace std;
#if 1
    // 1st example. Read from cin (Ctrl-Z for eof)
    deque<string> col1;
    copy(istream_iterator<string>(cin),
        istream_iterator<string>(),
        back_inserter(col1));

    cin.clear();

    // 2nd example. Separate define stack params
    // Read from cin (Ctrl-Z for eof)
    // Actually the same
    deque<string> col2;
    istream_iterator<string> start_param(cin);
    istream_iterator<string> end_param;
    copy(start_param, end_param, back_inserter(col2));

    cin.clear();
#endif
    // 3rd example. Function instead of action
    // do nothing
    // warning C4930: 'std::deque<std::string,std::allocator<_Ty>> ...
    // prototyped function not called (was a variable definition intended?)
    deque<string> col3(istream_iterator<string>(cin),
        istream_iterator<string>());
}

// Ch 3 Task 30 Loosing accuracy on float large numbers
void show_float_double(){
    
    typedef double real;
    
    // changing to float we loosing accuracy
    // 10^8 accuracy more then 1, so for float N = 10^8, N-1 == N
    // we turn into endless cycle
    //typedef float real;

    // Could be endless!
    real x = 1e8;
    while (x > 0){
        --x;
    }
}

int main(){

    show_bit_buffer();
    show_function_declare();
    show_float_double();

    return 0;
}
