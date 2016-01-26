#include <iostream>
#include <vector>
#include <boost/any.hpp>

// boost.any examples
// use boost::any containers for different types starage
// use boost::any_cast to extract value

template < typename T >
T extract(const boost::any& a){
    
    try {
        // unpack stored values
        return boost::any_cast<T>(a);
    }
    catch(const boost::bad_any_cast &){
        std::cout << "Bad cast" << std::endl;
    }
}

int main(int argc, char* argv[]) {

    int intval = 1;
    long longval = 2;
    float fval = 1.5;
    double dblval = 2.5;
    const char* chval = "str";
    boost::any a(1);
    boost::any none;

    std::cout << "Is empty (a): " << a.empty() << std::endl;
    std::cout << "Is empty (none): " << none.empty() << std::endl;


    std::cout << "Is int (a): " << (a.type() == typeid(int)) << std::endl;
    std::cout << "Is int (none): " << (a.type() == typeid(int)) << std::endl;


    std::vector<boost::any> different;
    different.push_back(intval);
    different.push_back(longval);
    different.push_back(fval);
    different.push_back(dblval);
    different.push_back(chval);
    different.push_back(a);
    different.push_back(none);

    try {
        // unpack stored values
        int s = boost::any_cast<int>(a);
        float f = boost::any_cast<float>( different.at(2) );
        std::cout << "'a' converted to int: " << s 
            << "; extract fval: " << f 
            << "; extract dblval: " << extract<double>(dblval) << std::endl;
    }
    catch(const boost::bad_any_cast &){
        std::cout << "Bad cast" << std::endl;
    }

    return 0;
}
