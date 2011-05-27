#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <boost/lexical_cast.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

unsigned find_r(unsigned n){

}

bool is_power(unsigned n){

}

bool is_prime(unsigned n){

    // 1. check if n = a^b form -> COMPOSITE.
    if( is_power(n) ){
        return false;
    }

    // 2. Find the smallest r such that Or(n) > 4 log^2(n)
    unsigned r = find_r(n);
    if(r == 0){
        return false;
    }
    if(r >= n){
        return true;
    }

    //  if 1 < GCD(a, n) < n for all a <= r  -> COMPOSITE.



}

void get_permutations(std::string& numbers){
    std::sort(numbers.begin(), numbers.end());
    do{
        unsigned check_me = boost::lexical_cast<unsigned>(numbers);
        if( (check_me % 2) == 1  ){
            // todo : threaded pool
            if(isPrime(check_me)){
                cout << check_me << endl;
            }
                
        }
    }
    while( std::next_permutation(numbers.begin(), numbers.end()) );
}

int main(int argc, char* argv[]) {


    if (argc != 2) {
        cout << "Usage: primetest <numbers>" << endl;
    }

#if 0
    try{
        string strnums( argv[1] );
        get_permutations(strnums);

        cout << endl;
    }
    catch(boost::bad_lexical_cast& e){
        cerr << e.what() << "(not a number?)" << endl;
    }
#else
    string strnums(argv[1]);
    unsigned check_me = boost::lexical_cast<unsigned>(strnums);
    if (isPrime(check_me)) {
        cout << check_me << " is prime" << endl;
    }
#endif

    return 0;
}

