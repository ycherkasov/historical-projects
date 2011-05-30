/* 
 * File:   primetest_simple.h
 * Author: straight
 *
 * Created on 29 Май 2011 г., 11:22
 */

#ifndef PRIMETEST_SIMPLE_H
#define	PRIMETEST_SIMPLE_H

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>

NTL_CLIENT

typedef NTL::ZZ ntl_bigint;
typedef NTL::ZZ_p ntl_mod_bigint;
typedef NTL::ZZ_pX ntl_polynome;

class aks_test1 {
public:

    /** @brief Assign a number to check is it prime */
    aks_test1(unsigned n) : _number( NTL::to_ZZ(n) ), _log2n(NTL::NumBits(_number)) {}

    bool operator()() {

        if (is_power())
            return false;

        ZZ r = find_r();

        if (r == 0)
            return false;
        if (r >= _number)
            return true;

        /////////////////////////
        ntl_mod_bigint::init(_number);
        ntl_polynome left_polynome, right_polynome, temp_polynome;
        
        //check for greater values out of range of long
        long limit = NTL::to_long( SqrRoot(r) * _log2n );

        //rhs = x^n%r
        long modulo_r = NTL::to_long(_number % r);
        SetCoeff(right_polynome, modulo_r, 1);

        //p = x
        SetCoeff(temp_polynome, 1, 1);

        for (long i = 1;i <= limit;i++) {
            //rhs = X^n%r+i
            SetCoeff(right_polynome, 0, i);

            //p = x+i;
            SetCoeff(temp_polynome, 0, i);

            left_polynome = build_polynome(r, temp_polynome);
            if (left_polynome != right_polynome) {
                return false;
            }
        }
        return true;
    }

    inline bool is_power() {
        ZZ upper_bound, lower_bound, temp, the_power;
        upper_bound = _number;
        lower_bound = 1;
        for (long i = 1;i < NumBits(_number);i++) {
            while ((upper_bound - lower_bound) > 1) {
                temp = (upper_bound + lower_bound) / 2;

                the_power = NTL::power(temp, i + 1);

                if (the_power == _number) {
                    return 1;
                }
                (the_power > _number) ? (upper_bound = temp) : (lower_bound = temp);
            }
        }
        return 0;
    }

    inline ZZ find_r() {
        ntl_bigint q, r;
        ntl_bigint zero(NTL::to_ZZ(0));
        ntl_bigint j;
        ntl_bigint mods, nmodq, jmodq;
        bool foundr;
        q = _log2n + 1;
        
        while (true) {
            foundr = true;
            for (j = 1;j <= _log2n;j++) {
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
        ZZ a;
        ZZ gcd;
        for (a = 2;a < r;a++) {
            GCD(gcd, a, _number);
            if (!NTL::IsOne(gcd)) {
                return zero;
            }
        }
        return r;
    }
#if 0
    inline long mod(long i, long r) {
        // Returns the value of i mod r
        long ret = i;
        while (ret >= r) ret = ret - r;
        return ret;
    }
#endif
    inline void ModuloExponents(ZZ_pX &p, const ZZ &r) {
        //p % x^r-1
        long i = deg(p);
        long rlong;
        long imodr;
        ZZ_p coff;
        ZZ_p newcoff;

        conv(rlong, r);//converting r to long as coefficient

        while (i >= rlong) {
            coff = coeff(p, i);
            if (!IsZero(coff)) {
                
                //imodr = mod(i, rlong);
                imodr = i%rlong;

                // Add the values of same power coefficient
                newcoff = coeff(p, imodr);
                add(newcoff, newcoff, coff);

                // set the coeff to
                SetCoeff(p, imodr, newcoff);
                SetCoeff(p, i, 0);
            }
            i--;
        }
    }

    inline ZZ_pX build_polynome(ZZ r, ZZ_pX p) {
        long longr;
        conv(longr, r);
        ZZ_pX f;
        f = 1;
        for (long i = _log2n;i != 0;i--) {
            sqr(f, f);
            if (bit(_number, i - 1) == 1) {
                mul(f, f, p);
            }
            ModuloExponents(f, r);
        }
        return f;
    }


private:
    const ntl_bigint _number;
    long _log2n;

};

#endif	/* PRIMETEST_SIMPLE_H */

