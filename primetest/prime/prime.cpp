#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "primetest_ntl.h"


using std::cout;
using std::cerr;
using std::endl;
using std::string;

/// Count all prime numbers from permutations
void get_permutations(std::string& numbers) {

    std::sort(numbers.begin(), numbers.end());
    do {
        if(*numbers.begin() == '0')
            break;
        unsigned check_me = boost::lexical_cast<unsigned>(numbers);
        if ((check_me % 2) == 1) {

            // todo : threaded pool

            aks_test aks(check_me);
            if (aks()) {
                cout << check_me << endl;
            }

        }
    }
    while (std::next_permutation(numbers.begin(), numbers.end()));
}

/// Just check primality
void check_primality(unsigned check_me) {

	aks_test aks(check_me);
	if (aks.is_prime()) {
		cout << check_me << " is prime" << endl;
	}
}

// Очень хотелось написать многопоточную версию, но к сожалению NTL не thred-safe
#if NTL_WOULD_BE_THREAD_SAFE
void get_permutations_parallel(std::string& numbers) {

    std::sort(numbers.begin(), numbers.end());
    tbb::concurrent_vector<unsigned> out;
    tbb::task_list list;
    do {
        unsigned check_me = boost::lexical_cast<unsigned>(numbers);
        cout << "check me: " << check_me << endl;
        if ((check_me % 2) == 1) {
            long_task* a = new (tbb::task::allocate_root())long_task(check_me, out);
            list.push_back(*a);
        }
    }
    while (std::next_permutation(numbers.begin(), numbers.end()));

    tbb::task::spawn_root_and_wait(list);
    list.clear();
}
#endif


int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Usage: primetest <numbers>" << endl;
        return 0;
    }

    string strnums;
    double sttime = NTL::GetTime();
	unsigned check_me = 0;

    try {
        strnums.assign(argv[1]);
		check_me = boost::lexical_cast<unsigned>(strnums);
    }
    catch (boost::bad_lexical_cast& e) {
        cerr << e.what() << "(not a number?)" << endl;
    }

#if 0
    get_permutations(strnums);
#endif

	check_primality(check_me);
    cout << "The Time taken in checking primality is " << NTL::GetTime() - sttime << endl;
    cout << endl;

    return 0;
}

