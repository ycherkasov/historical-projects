// The minimal C++ program
// is the program’s return value to the system.
// If no value is returned, the system will receive a value indicating successful completion.
#if 0
int main() {}
#endif

//Flowing off the end of a function is equivalent to a return with no value;
//this results in undefined behavior in a value - returning function.


#include <cstdio>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <complex>
#include <valarray>

using namespace std;

// Show output iterator
void show_out_iter() {
    // Stream as a container example
    // Assigning iterator == out to the console
    ostream_iterator<string> oi(cout);
    *oi = "aaa";
    ++oi;
    *oi = " bbb\n";
}

// Show iterator that refers to the input stream
// String that assigns passed from the keyboard
void show_istr_iter() {

    // request for the first string
    istream_iterator<string> iinput_iter(cin);

    // iterator has that value
    string s1 = *iinput_iter;

    // request for the second string
    ++iinput_iter;

    // iterator has that value
    string s2 = *iinput_iter;

    // Result
    cout << "First string: " << s1 << '\n' << "Second string: " << s2 << endl;
}

void show_math() {

    complex<double> c1(1., 2.);
    complex<double> c2(1.5, 0.2);
    complex<double> c3 = c1 + c2;

    valarray<double> vect1(10, 2);
    valarray<double> vect2(5, 2);
    valarray<double> vect3 = vect1 + vect2;

    c3 = pow(c3, 2);

    vect3 = vect1*vect2;

    c2 = log(c1);
}

// C-style fractal
void frac(int k)
{
    float i, j, r, y = -16;
    while (puts(""), y++ < 15){
        for (int x = 0; x++ < 84; putchar(" .:-;!/>)|&IH%*#"[k & 15])){
            for (i = k = r = 0; j = r*r - i*i - 2 + x / 25, i = 2 * r*i + y / 10, j*j + i*i < 11 && k++ < 111; r = j);
        }
    }
}

int main() {

    // main try/catch block does not let go any exception unhanded
    try{
        frac(10);
        show_out_iter();
        show_istr_iter();
        show_math();
    }
    catch (const std::exception& e){
        cout << "exception: " << e.what();
        return 1;
    }

    // retirn that code to the OS
    return 0;
}
