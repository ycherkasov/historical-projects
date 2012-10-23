#ifndef PRIMETEST_MT_NTL_H
#define PRIMETEST_MT_NTL_H

#include <string>
#include <stdexcept>
#include "uint-big.h"
#include "polynome-big.h"

class aks_test_mt{
public:
    
    aks_test_mt(const big_int& n)
        : _number(n)
        , _log2n(n.num_bits()){
            _log2n = n.num_bits();
        }
    
    bool is_prime(){
        
        // 1. check if n = a^b form -> COMPOSITE.
        if (is_power())
            return false;

        // 2. Find the smallest r such that Or(n) > 4 log^2(n)
        big_int r = find_r();

        if (r.is_zero())
            return false;
        if (r >= _number)
            return true;

        // 3. Compare (x-i)^n == (x^n- 1)(mod x^r - 1, n)
        polynome_big left_polynome;
        polynome_big right_polynome;
        polynome_big temp_polynome;
        

        // check for greater values out of range
        // sqr(r)*log2(n)
        ulong limit = r.sqr_root().get_long() * _log2n;

        // right = x^n%r
        big_int modulo = _number % r;
        long modulo_r = modulo.get_long();
        right_polynome.set_coeff(modulo_r, 1);

        // temp = x
        temp_polynome.set_coeff(1, 1);
        

        // compare polynome pair
        for (long i = 1; i <= limit; i++) {
        }
        return true;
    }


    
public:
//protected:

    bool is_power(){
        big_int upper_bound(_number);
        big_int lower_bound(1);
        big_int temp, the_power;

        for (ulong i = 1; i < _log2n; i++) {
            while ((upper_bound - lower_bound) > 1) {
                
                temp = (upper_bound + lower_bound) / 2;
                
                the_power = temp;
                the_power.power(i + 1);

                if (the_power == _number) {
                    return true;
                }

                (the_power > _number) ? (upper_bound = temp) : (lower_bound = temp);
            }
        }
        return false;
    }

    big_int find_r(){
        big_int q, r;
        big_int zero;
        //uint_big j;
        long j;
        big_int mods, nmodq, jmodq;
        bool foundr;
        q = _log2n + 1;

        while (true) {
            foundr = true;
            for (j = 1; j <= _log2n; j++) {
                //nmodq = n%q
                //NTL::rem(nmodq, _number, q);
                nmodq = _number % q;

                //jmodq = j%q
                //NTL::rem(jmodq, j, q);
                jmodq = j % q;

                //NTL::PowerMod(mods, nmodq, jmodq, q);
                mods = nmodq;
                mods.power_mod(jmodq, q);
                        
                //if (NTL::IsOne(mods)) {
                if (mods.is_one()) {
                    foundr = false;
                    break;
                }
            }
            if (foundr) {
                r = q;
                break;
            }
            q++;
        }
        if (r >= _number)
            return _number;
        big_int a;
        big_int gcd;
        for (a = 2; a < r; a++) {
            gcd = a.gcd(_number);
            if (!gcd.is_one()) {
                return zero;
            }
        }
        return r;
    }
    
    //void modulo_exponents(polynome_t &p, const bigint_t &r){
    //}
    
    //polynome_t build_polynome(bigint_t r, polynome_t p);
    
private:
    const big_int _number;
    long _log2n;
};


#endif //PRIMETEST_MT_NTL_H
