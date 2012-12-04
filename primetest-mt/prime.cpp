#include <iostream>
#include <cassert>
#include <boost/lexical_cast.hpp>
#include "prime-mt.h"

using std::string;
using std::cout;
using std::endl;


extern "C"{
#include "flint.h"
#include "fmpz_poly.h"
}

void test_bigint() {

    std::cout << "Initialization" << endl;
    {
        big_int z;
        big_int a(100);
        big_int b(250);
        big_int big("9999999999999999999999999");

        cout << "a = " << a.get_string() << endl;
        cout << "b = " << b.get_string() << endl;
        cout << "z = " << z.get_string() << endl;
        cout << "big = " << big.get_string() << endl;

        big_int c(a);
        big_int d(big);
        a = b;

        cout << "a = " << a.get_string() << endl;
        cout << "b = " << b.get_string() << endl;
        cout << "c = " << c.get_string() << endl;
        cout << "d = " << d.get_string() << endl;
        cout << "z = " << z.get_string() << endl;
        cout << "big = " << big.get_string() << endl;

        assert(c == 100);
        assert(a == b);
        assert(b == a);
        assert(d == big);
        assert(big == d);
    }

    std::cout << "Random generation" << endl;
    {
        big_int a;
        big_int b;
        big_int big("9999999999999999999999999");

        a.random_init(1000, 2000);
        b.random_init(1000000, big);

        cout << "a = " << a.get_string() << endl;
        cout << "b = " << b.get_string() << endl;

        assert(a > 1000);
        assert(a < 2000);
        assert(b > 1000000);
        assert(b < big);
    }

    std::cout << "Conversions" << endl;
    {
        big_int a(100);
        big_int b(200);
        big_int big("9999999999999999999999999");

        cout << "a = " << a.get_string() << endl;
        cout << "b = " << b.get_string() << endl;
        cout << "big = " << big.get_string() << endl;

        assert(a.get_long() == 100);
        assert(b.get_long() == 200);
        assert(big.get_string().compare("9999999999999999999999999") == 0);
    }

    std::cout << "Properties" << endl;
    {
        big_int z;
        big_int o(1);
        
        big_int a(100);
        big_int b(999);
        big_int big("9999999999999999999999999");

        cout << "a = " << a.get_string() << endl;
        cout << "b = " << b.get_string() << endl;
        cout << "big = " << big.get_string() << endl;
        
        //size_t s1 = a.num_digits();
        //size_t s2 = b.num_digits();
        
        // b == 999
        // WTF? (s2 == 4)
        //assert(s1 == s2);
        
        //size_t s3 = big.num_digits();
        // WTF? (s3 == 26)
        //assert(s3 == 25);
        
        size_t s1 = a.num_bits();
        size_t s2 = b.num_bits();
        
        size_t s3 = big.num_bits();
        
        assert(s1 == 7);
        assert(s2 == 10);
        assert(s3 == 84);
        
        assert(a.is_even());
        assert(!a.is_odd());
        assert(!big.is_even());
        assert(big.is_odd());
        
        assert(z.is_zero());
        assert(o.is_one());
    }
    
    std::cout << "Comparasions" << endl;
    {
        big_int a1(100);
        big_int a2(100);
        big_int b1(250);
        big_int b2(250);
        big_int big("9999999999999999999999999");
        
        assert(a1 < b1);
        assert(b1 > a1);
        assert(big > a1);
        assert(big > b1);

        assert(a1 <= b1);
        assert(b1 >= a1);
        assert(a1 <= a2);
        assert(b1 >= b1);
        assert(a1 == a2);
        assert(b1 == b1);
        assert(a1 != b1);
        assert(b1 != a1);
    }
    
    std::cout << "Arithmetic operations" << endl;
    {
        big_int z;
        big_int a(250);
        big_int b(100);
        big_int big("9999999999999999999999999");

        big_int c = a + b;
        cout << "c = a + b = " 
                << a.get_string() 
                << " + "
                << b.get_string() 
                << " = "
                << c.get_string() 
                << endl;
        assert(c == big_int(350));

        c = a - b;
        cout << "c = a - b = " 
                << a.get_string() 
                << " - "
                << b.get_string() 
                << " = "
                << c.get_string() 
                << endl;
        assert(c == big_int(150));

        c = a * b;
        cout << "c = a * b = " 
                << a.get_string() 
                << " * "
                << b.get_string() 
                << " = "
                << c.get_string() 
                << endl;
        assert(c == big_int(25000));

        c = a / b;
        cout << "c = a / b = " 
                << a.get_string() 
                << " / "
                << b.get_string() 
                << " = "
                << c.get_string() 
                << endl;
        assert(c == big_int(2));
        
        // check the same behaviour as a long value
        long l = 250/100;
        assert(c == l);

        c = a % b;
        cout << "c = a % b = " 
                << a.get_string() 
                << " % "
                << b.get_string() 
                << " = "
                << c.get_string() 
                << endl;
        assert(c == big_int(50));
                
        // check the same behaviour as a long value
        long l1 = 250 % 100;
        assert(c == l1);
                
        big_int a1(a);
        big_int b1(b);
        assert(++a1 == 251);
        assert(++a1 == 252);
        assert(b1++ == 100);
        assert(b1++ == 101);

        big_int a2(a);
        big_int b2(b);
        assert(--a2 == 249);
        assert(a2-- == 249);
        assert(a2 == 248);
        assert(b2-- == 100);
        assert(b2-- == 99);
        assert(b2 == 98);
        
        big_int a3 = -a;
        assert(a3 == (-250));
    }
    

    std::cout << "Modifying operations" << endl;
    {
        big_int z;
        big_int a(250);
        big_int b(100);
        big_int big("9999999999999999999999999");

        {
            big_int a1(a);
            big_int b1(b);
            a1.power(2);
            b1.power(0);
            assert(a1 == 250 * 250);
            assert(b1 == 1);
        }

        {
            big_int a2(a);
            big_int b2(b);
            big_int a3 = big_int::power(a2, 2);
            big_int b3 = big_int::power(b2, 0);
            long c = 250 * 250;
            assert(a3 == c);
            assert(b3 == 1);
        }

        {
            big_int a4(a);
            big_int b4(b);
            a4.power_mod(2, 1);
            b4.power_mod(0, 5);
            //todo: assert(a1 == 250*250);
            assert(b4 == 1);
        }


        {        
            big_int a5(400);
            big_int b5(10000);
            a5.sqr_root();
            big_int c = big_int::sqr_root(b5);
            assert(a5 == 20);
            assert(c == 100);
        }
        
        {
            big_int a7(a);
            big_int b7(b);
            big_int c1 = a7.gcd(b7);
            big_int c2 = big_int::gcd(a7, b7);
            assert(c1 == 50);
            assert(c1 == c2);
        }
        

    }
}

void test_polynome() {


    std::cout << "Initializing" << endl;
    polynome_big p1;
    p1.set_coeff(3, 5);
    p1.set_coeff(0, -1);
    std::cout << "p1 = " << p1.get_string() << endl;


    polynome_big p2;
    p2.set_coeff(2, 7);
    p2.set_coeff(1, -2);
    p2.set_coeff(0, 1);
    std::cout << "p2 = " << p2.get_string() << endl;
    
    std::cout << "+ - *" << endl;
    {
        polynome_big p11(p1);
        polynome_big p21(p2);
        
        polynome_big a = p11 + p21;
        cout << "p1 + p2 = " 
                << p1.get_string()
                << " + "
                << p2.get_string()
                << " = "
                << a.get_string()
                << endl;
        
        polynome_big res1;
        res1.set_coeff(3, 5);
        res1.set_coeff(2, 7);
        res1.set_coeff(1, -2);
        assert(a == res1);
        
        polynome_big b = p11 - p21;
        cout << "p1 - p2 = " 
                << p1.get_string()
                << " - "
                << p2.get_string()
                << " = "
                << b.get_string()
                << endl;
        
        polynome_big res2;
        res2.set_coeff(3, 5);
        res2.set_coeff(2, -7);
        res2.set_coeff(1, 2);
        res2.set_coeff(0, -2);
        assert(b == res2);
        
                
        polynome_big c = p11 * p21;
        cout << "p1 * p2 = " 
                << p1.get_string()
                << " * "
                << p2.get_string()
                << " = "
                << c.get_string()
                << endl;
        
                
        polynome_big res3;
        res3.set_coeff(5, 35);
        res3.set_coeff(4, -10);
        res3.set_coeff(3, 5);
        res3.set_coeff(2, -7);
        res3.set_coeff(1, 2);
        res3.set_coeff(0, -1);
        assert(c == res3);
    }

    std::cout << "Scalar operations" << endl;
    {

        big_int t("666666666666666666666");
        polynome_big p31(p1);
        polynome_big p32(p2);
        
        polynome_big a = p31.scalar_mul(10);
        
        
        cout << "Scalar mul for "
                << p1.get_string()
                << " and "
                << 10
                << " is "
                << a.get_string()
                << endl;
        
                
        polynome_big res2;
        res2.set_coeff(3, 50);
        res2.set_coeff(0, -10);
        assert(a == res2);
        
        polynome_big b = p32.scalar_mul(t);
        cout << "Scalar mul for "
                << p2.get_string()
                << " and "
                << t.get_string()
                << " is "
                << b.get_string()
                << endl;
    }
    //polynome_big p1("2  1/2 1/3");
    //std::cout << "p1 = " << p1.get_string() << endl;
    
}



void test_ispower() {

    big_int a(100);
    big_int b(131);

    aks_test_mt t1(a);
    aks_test_mt t2(b);

    cout << "t1.is_power = " << t1.is_power() << endl;
    cout << "t2.is_power = " << t2.is_power() << endl;
}

void test_aks() {

    //big_int a(100);
    big_int b(131);

    //aks_test_mt t1(a);
    aks_test_mt t2(b);
    
    bool pr = t2.is_prime();

}

int main(int argc, char* argv[]) {

#if 0
    if (argc != 2) {
        std::cout << "Usage: program <number>" << std::endl;
    }
#endif
    try {

        //test_bigint();
        //test_polynome();
        //test_ispower();
        test_aks();
        

    }    
    catch (const boost::bad_lexical_cast& e) {
        cout << e.what() << endl;
    }    
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }


    return 0;
}
