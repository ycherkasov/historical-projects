#ifndef _AUTO_H_
#define _AUTO_H_

#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>

/////////////////////////////////////////////////////////////////
/// auto keyword
#if (__GNUC__ > 3) && (__GNUC_MINOR__ > 3) ||  (_MSC_VER > 1500) // since gcc 4.4 or MSVC 2010
  
/// \brief print all vector elements using C++0x keyword auto
template<class T>
void printall_auto(const std::vector<T>& v) {
    for (auto p = v.begin(); p != v.end(); ++p){
        std::cout << (*p) << "\n";
    }
}

/// \brief show C++0x keyword 'auto'
void show_auto(){
    using namespace boost::assign;
    std::vector<int> v;
    v += 1, 3, 5, 7;
    std::cout << "print using auto: \n";
    printall_auto(v);
}
#endif
/////////////////////////////////////////////////////////////////
#endif //_AUTO_H_