#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "primetest_ntl.h"
#include "primetest_simple.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

//template <typename Func>
void get_permutations(std::string& numbers) {
    std::sort(numbers.begin(), numbers.end());
    do {
        unsigned check_me = boost::lexical_cast<unsigned>(numbers);
        if ((check_me % 2) == 1) {
            // todo : threaded pool
#if 0
            aks_test aks(check_me);
            if (aks()) {
#else
            if(isPrimeAKSFaster(NTL::to_ZZ(check_me))){
#endif
                cout << check_me << endl;
            }

        }
    }
    while (std::next_permutation(numbers.begin(), numbers.end()));
}

int main(int argc, char* argv[]) {


    if (argc != 2) {
        cout << "Usage: primetest <numbers>" << endl;
    }

#if 1
    try {
        string strnums(argv[1]);
        //get_permutations<aks_test>(strnums);
        double sttime;
        sttime = NTL::GetTime();

        get_permutations(strnums);
        cout << "The Time taken in checking primality is " << NTL::GetTime() - sttime << endl;
        cout << endl;
    }
    catch (boost::bad_lexical_cast& e) {
        cerr << e.what() << "(not a number?)" << endl;
    }
#else
    string strnums(argv[1]);
    unsigned check_me = boost::lexical_cast<unsigned>(strnums);
    NTL::INIT_VAL_STRUCT t = {};
    ntl_bigint bcheckme(t, check_me);
    if (aks_test(bcheckme)) {
        cout << check_me << " is prime" << endl;
    }
    else {
        cout << check_me << " is composite" << endl;
    }
#endif

    return 0;
}

