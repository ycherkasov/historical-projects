#include <iostream>
#include <boost/lexical_cast.hpp>
#include "prime-mt.h"

using std::string;
using std::cout;
using std::endl;

void test_bigint(){
    
    uint_big z;
    uint_big a(100);
    uint_big b(250);
    uint_big big("9999999999999999999999999");
    
    uint_big c = a + b;
    cout << "c = a + b = " << c.get_string() << endl;
    
    c = b - a;
    cout << "c = b - a = " << c.get_string() << endl;
    
    c = a * b;
    cout << "c = a * b = " << c.get_string() << endl;
    
    c = b / a;
    cout << "c = b / a = " << c.get_string() << endl;
    
    unsigned long ltest = 250/100;
    
    bool is_eq = (a == big);
    bool is_neq = (b != big);
    
    a = b;

    is_eq = (a == b);
    is_neq = (a != b);
    
    uint_big cp(a);
    
    ulong l = cp.get_ulong();
    string s = b.get_string();
    s = big.get_string();

    size_t digits = big.num_digits();
    size_t bits = big.num_bits();
    
    bool is_even = (big.is_even());
    bool is_odd = (big.is_odd());
    
    bool greater = (big > a);
    bool less = (big < a);
}

void test_ispower(){
        
    uint_big a(100);
    uint_big b(131);
    
    aks_test_mt t1(a);
    aks_test_mt t2(b);
    
    cout << "t1.is_power = " << t1.is_power() << endl;
    cout << "t2.is_power = " << t2.is_power() << endl;
}

int main(int argc, char* argv[]) {
    
#if 0
    if (argc != 2) {
        std::cout << "Usage: program <number>" << std::endl;
    }
    try{
    }
    catch(const boost::bad_lexical_cast& e){
    }
    catch(const std::exception& e){
    }
#endif
    
    //test_bigint();
    //test_ispower();
    
    return 0;
}
