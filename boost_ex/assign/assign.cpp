#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <boost/assign/std/vector.hpp> // for 'operator+=()'
#include <boost/assign/list_inserter.hpp> // for 'push_front()'
#include <boost/assign/list_of.hpp> // for 'list_of()'

// Demonstrates abilities of boost.assign
// e.g. simplify setting init values to standard containers

using std::vector;

template <typename T>
void print_item(const T& t) {
    std::cout << t << " ";
}

template <>
void print_item(const std::pair<int, int>& t) {
    std::cout << " f=" << t.first << " s= " << t.second << " ";
}

// assign series to vector
void test_vector() {
    using namespace boost::assign;// bring 'operator+=()' into scope
    vector<int> values;
    values += 1, 2, 3, 4, 5, 6, 7, 8, 9;// insert values at the end of the container
    std::for_each(values.begin(), values.end(), print_item<int>);
    std::cout << std::endl;
}

void test_listof(){
    using namespace boost::assign;
    const std::list<int> values = list_of(2)(3)(5)(7)(11);
    std::for_each(values.begin(), values.end(), print_item<int>);
    std::cout << std::endl;
}

void test_map(){
    using namespace boost::assign;
    std::map<int,int> values = map_list_of(1,2)(2,3)(3,4)(4,5)(5,6);
    std::for_each(values.begin(), values.end(), print_item< std::pair<int, int> >);
    std::cout << std::endl;
}

void test_tuple(){
    typedef boost::tuple<int,std::string,int> tuple;
    std::vector<tuple> v = boost::assign::tuple_list_of( 1, "foo", 2 )( 3, "bar", 4 );
}

template< class T >
struct next
{
    T seed;
    next( T seed ) : seed(seed)
    { }

    T operator()()
    {
        return seed++;
    }
};

void assign_repeated_sequence(){
    using namespace boost::assign;
    std::vector<int> values;

    // push 4x10 sequence
    values += 1,2,3,repeat(10,4),5,6,7,8,9;
    std::for_each(values.begin(), values.end(), print_item< int>);
    std::cout << std::endl;

    // push back two function-generated sequences
    values += 1,2,repeat_fun(4,&rand),4;
    push_back( values ).repeat_fun(4,next<int>(0))(4).repeat_fun(4,next<int>(5));
    std::for_each(values.begin(), values.end(), print_item< int>);
    std::cout << std::endl;

}

int main(int argc, char* argv[]) {


    //test_vector();
    //test_listof();
    //test_map();
    //test_tuple();
    assign_repeated_sequence();

    return 0;
}
