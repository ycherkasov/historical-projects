#ifndef _RANGE_FOR_H_
#define _RANGE_FOR_H_

#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>

/////////////////////////////////////////////////////////////////
/// Range-based for
#if (__GNUC__ > 3) && (__GNUC_MINOR__ > 5) // since 4.6
/// \brief print all vector elements using C++0x range :
template<class T>
void printall_range(vector<T>& v) {
    for (auto x : v) cout << x << '\n';
}

/// \brief modify all vector elements using C++0x range : and reference
template<class T>
void modify_range(vector<T>& v) {
    // using a reference to allow us to change the value
    for (auto& x : v) ++x; 
}

void show_ranges(){
    using namespace boost::assign;
    vector<int> v;
    v += 1, 3, 5, 7;
    cout << "print using ranges: \n";
    printall_range(v);
    cout << "modify using ranges: \n";
    modify_range(v);
}
#endif
/////////////////////////////////////////////////////////////////

#endif//_RANGE_FOR_H_