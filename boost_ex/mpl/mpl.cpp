#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/aux_/preprocessed/gcc/vector_c.hpp>

using namespace boost;
using std::cout;
using std::endl;

struct S1{
    int a;
    int b;
};

struct S2{
    int a;
};

typedef boost::mpl::vector_c<S1, 50, 0> type1_t;
typedef boost::mpl::vector_c<S2, 10, 5> type2_t;

int main(int argc, char* argv[]) {
    
//    if (argc != 2) {
//        std::cout << "Usage: program <number>" << std::endl;
//    }
    try{
        cout << mpl::plus<mpl::int_<2>, mpl::int_<3> >::type::value << endl;
    }
    catch(const boost::bad_lexical_cast& e){
    }
    catch(const std::exception& e){
    }
    return 0;
}
