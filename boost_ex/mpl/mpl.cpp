#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>

using namespace boost;
using std::cout;
using std::endl;

struct S1 {
    int a;
    int b;
};

struct S2 {
    int a;
};

typedef mpl::vector<S1, mpl::int_<50>, mpl::int_<0> > types;

int main(int argc, char* argv[]) {

    try {
        cout << mpl::plus<mpl::int_ < 2 >, mpl::int_ < 3 > >::type::value << endl;

        BOOST_MPL_ASSERT((is_same< mpl::at_c<types, 0 >::type, S1 >));

        cout << mpl::at_c<types, 1 >::type::value << endl;
    }
    catch (const boost::bad_lexical_cast& e) {
    }
    catch (const std::exception& e) {
    }
    return 0;
}
