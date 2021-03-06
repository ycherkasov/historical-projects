/* 
 * File:   primetest_ntl.h
 * Author: straight
 *
 * Created on 29 Май 2011 г., 11:08
 */

#ifndef PRIMETEST_NTL_H
#define	PRIMETEST_NTL_H

#include <iostream>

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>

typedef NTL::ZZ ntl_bigint;
typedef NTL::ZZ_p ntl_mod_bigint;
typedef NTL::ZZ_pX ntl_polynome;

class aks_test {
public:

    /** @brief Assign a number to check is it prime */
    aks_test(unsigned n) : _number(NTL::to_ZZ(n)), _log2n(NTL::NumBits(_number)) {
		long tst = NTL::NumBits(_number);
		__asm nop
	}

    /** @brief Main test performed as operator()  */
    bool operator()() {
        return is_prime();
    }

    bool is_prime(){

        // 1. check if n = a^b form -> COMPOSITE.
        if (is_power())
            return false;

        // 2. Find the smallest r such that Or(n) > 4 log^2(n)
        ntl_bigint r = find_r();
		std::cout << "smallest r such that Or(n) > 4 log^2(n) = " << NTL::to_long(r);

        if (r == 0)
            return false;
        if (r >= _number)
            return true;

        // 3. Compare (x-i)^n == (x^n- 1)(mod x^r - 1, n)
        ntl_mod_bigint::init(_number);
        ntl_polynome left_polynome, right_polynome, temp_polynome;

        // check for greater values out of range
        // sqr(r)*log2(n)
        long limit = NTL::to_long(NTL::SqrRoot(r) * _log2n);

        // right = x^n%r
        long modulo_r = NTL::to_long(_number % r);
        NTL::SetCoeff(right_polynome, modulo_r, 1);

        // temp = x
        NTL::SetCoeff(temp_polynome, 1, 1);

        // compare polynome pair
        for (long i = 1; i <= limit; i++) {

            //right = X^n%r+i
            NTL::SetCoeff(right_polynome, 0, i);

            // t = x+i;
            NTL::SetCoeff(temp_polynome, 0, i);

            left_polynome = build_polynome(r, temp_polynome);
            if (left_polynome != right_polynome) {
                return false;
            }
        }
        return true;
    }

protected:

    /** @brief Check if number is in a^b form */
    bool is_power() {
        ntl_bigint upper_bound(_number), lower_bound(NTL::to_ZZ(1)), temp, the_power;

        for (long i = 1; i < NTL::NumBits(_number); i++) {
            while ((upper_bound - lower_bound) > 1) {
                
                temp = (upper_bound + lower_bound) / 2;
                the_power = NTL::power(temp, i + 1);

                if (the_power == _number) {
                    return true;
                }

                (the_power > _number) ? (upper_bound = temp) : (lower_bound = temp);
            }
        }
        return false;
    }

    /** @brief Find r coeff */
    ntl_bigint find_r() {
        ntl_bigint q, r;
        ntl_bigint zero(NTL::to_ZZ(0));
        ntl_bigint j;
        ntl_bigint mods, nmodq, jmodq;
        bool foundr;
        q = _log2n + 1;

        while (true) {
            foundr = true;
            for (j = 1; j <= _log2n; j++) {
                //nmodq = n%q
                NTL::rem(nmodq, _number, q);

                //jmodq = j%q
                NTL::rem(jmodq, j, q);

                NTL::PowerMod(mods, nmodq, jmodq, q);
                if (NTL::IsOne(mods)) {
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
        ntl_bigint a;
        ntl_bigint gcd;
        for (a = 2; a < r; a++) {
            gcd = NTL::GCD(a, _number);
            if (!NTL::IsOne(gcd)) {
                return zero;
            }
        }
        return r;
    }

    /** @brief Evaluate p % x^r-1 */
    void modulo_exponents(ntl_polynome &p, const ntl_bigint &r) {
        long i = NTL::deg(p);
        long imodr = 0;
        ntl_mod_bigint coff;
        ntl_mod_bigint newcoff;

        //converting r to long as coeff
        long rlong = NTL::to_long(r);

        while (i >= rlong) {
            coff = NTL::coeff(p, i);
            if (!NTL::IsZero(coff)) {

                imodr = i % rlong;

                // Add the values of same power coefficient
                newcoff = NTL::coeff(p, imodr);
                NTL::add(newcoff, newcoff, coff);

                // set the coeff to
                NTL::SetCoeff(p, imodr, newcoff);
                NTL::SetCoeff(p, i, 0);
            }
            i--;
        }
    }

    /** @brief Make left-hand polynome */
    ntl_polynome build_polynome(ntl_bigint r, ntl_polynome p) {
        ntl_polynome f;
        f = 1;
		std::cout << f << std::endl;
        for (long i = _log2n; i != 0; i--) {
            std::cout << f << std::endl;
			f = NTL::sqr(f);
			std::cout << f << std::endl;
            if (NTL::bit(_number, i - 1) == 1) {
                f = f * p;
				std::cout << f << std::endl;
            }
            modulo_exponents(f, r);
        }
        return f;
    }

private:
    const ntl_bigint _number;
    long _log2n;
};



#endif	/* PRIMETEST_NTL_H */

