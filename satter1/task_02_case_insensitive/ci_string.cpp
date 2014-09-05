#include <iostream>
#include <string>
#include <cassert>

using namespace std;

// Task 1.2-1.3 From Satter 1
// Case-insensitive string

// Override existiong char traits (non-polymorph way!)
// http://www.cplusplus.com/reference/string/char_traits/
struct ci_char_traits : public char_traits<char>
{
    // overload equality
    static bool eq(char c1, char c2){
        return (toupper(c1) == toupper(c2));
    }

    // overload strings compare (should be overload because eq/lt are non-virtual )
    static int compare (const char_type* p, const char_type* q, size_t n) {
        while (n--) {
            if ( !eq(*p,*q))
                return lt(*p,*q); ++p; ++q;
        }
        return 0;
    }

    // overload inequality
    static bool lt(char c1, char c2){
        return (toupper(c1) != toupper(c2));
    }

    // overload char search in string
    static const char* find(const char* s, size_t n, const char c){
        while( n-- > 0 && toupper(*s) != toupper(c) ) {
            ++s;
        }
        return n >= 0 ? s : 0;
    }

};

typedef basic_string<char, ci_char_traits> ci_string;

int main(){

    ci_string s("ABCDE");

    // case-insensitive
    assert(s == "ABCDE");
    assert(s == "abcde");

    // C-string: case-sensitive
    assert(strcmp(s.c_str(), "ABCDE") == 0);
    assert(strcmp(s.c_str(), "abcde") != 0);

    // ERROR! ostream(ostream&, ci_string&) should be overload!
    // cout << s << endl;

    return 0;
}
