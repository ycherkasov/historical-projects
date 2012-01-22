#include <iostream>

/// Greater common divisor implementation
/// return : Greater common divisor for 'm' and 'n'

unsigned gcd(const unsigned m, const unsigned n){
    unsigned n_(n);
    unsigned m_(m);

    while(n_){
        unsigned r = m_ % n_;
        m_ = n_;
        n_ = r;
    }
    return m_;
}

using namespace std;

int main()
{
    cout << "gcd(60, 24) = " << gcd(60, 24) << endl;
    cout << "gcd(1838, 7855) = " << gcd(1838, 7855) << endl;
    cout << "gcd(1, 17) = " << gcd(1, 17) << endl;
    return 0;
}