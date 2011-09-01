#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>

NTL_CLIENT


inline ZZ pow(ZZ base, ZZ exp) {
    ZZ p;
    p = 1;
    while (exp > 0) {
        if ((exp % 2) == 1) {
            p = p*base;
        }
        base = base*base;
        exp = exp / 2;
    }
    return p;
}

inline int isPower(ZZ n) {
    ZZ ub, lb, temp, tp, ZZ, i;
    ub = n;
    lb = 1;
    for (i = 1;i < NumBits(n);i++) {
        while ((ub - lb) > 1) {
            temp = (ub + lb) / 2;
            tp = pow(temp, i + 1);
            if (tp == n) {
                //cout << "The number " << n << " is not prime. " << n << " = " << temp << "^" << (i + 1) << "\n";
                return 1;
            }
            if (tp > n) {
                ub = temp;
            }
            else
                lb = temp;
        }
    }
    //cout << "The no. " << n << " is not of form a^b \n";
    return 0;
}

inline ZZ findR(ZZ n) {
    ZZ logn2;
    ZZ q, r, zero;
    zero = 0;
    logn2 = NumBits(n);
    sqr(logn2, logn2);
    ZZ j;
    ZZ mods, nmodq, jmodq;
    bool foundr;
    q = logn2 + 1;
    while (1) {
        foundr = true;
        for (j = 1;j <= logn2;j++) {
            rem(nmodq, n, q);
            rem(jmodq, j, q);
            PowerMod(mods, nmodq, jmodq, q);
            if (IsOne(mods)) {
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
    if (r >= n)
        return n;
    ZZ a;
    ZZ gcd;
    for (a = 2;a < r;a++) {
        GCD(gcd, a, n);
        if (!IsOne(gcd)) {
            return zero;
        }
    }
    return r;
}

inline long mod(long i, long r) {
    // Returns the value of i mod r
    long ret = i;
    while (ret >= r) ret = ret - r;
    return ret;
}

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
            imodr = mod(i, rlong);

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

inline ZZ_pX buildLHS(ZZ n, ZZ r, ZZ_pX p, long a, long lgn) {
    long longr;
    conv(longr, r);
    ZZ_pX f;
    f = 1;
    for (long i = lgn;i != 0;i--) {
        sqr(f, f);
        if (bit(n, i - 1) == 1) {
            mul(f, f, p);
        }
        ModuloExponents(f, r);
    }
    return f;
}

int isPrimeAKSFaster(ZZ n) {
    if (isPower(n))
        return 0;
    ZZ r;
    r = findR(n);

    //cout << endl << "the value of r is " << r << endl;
    if (r == 0)
        return 0;
    if (r >= n)
        return 1;
    /////////////////////////
    ZZ_p::init(n);
    ZZ_pX lhs, rhs, p;
    long i;
    long longr;

    //check for greater values out of range of long
    conv(longr, r);

    long lgn = NumBits(n);
    ZZ sqrtr;
    sqrtr = SqrRoot(r);

    long limit = 0;
    conv(limit, (sqrtr * lgn));

    long nmodr = 0;
    conv(nmodr, n % r);

    //rhs = x^n%r
    SetCoeff(rhs, nmodr, 1);
    
    //p = x
    SetCoeff(p, 1, 1);
    int flag = 1;

    for (i = 1;i <= limit;i++) {
        //rhs = X^n%r+i
        SetCoeff(rhs, 0, i);

        //p = x+i;
        SetCoeff(p, 0, i);
        
        lhs = buildLHS(n, r, p, i, lgn);
        if (lhs != rhs) {
            flag = 0;
            break;
        }
    }
    //cout << endl;
    return flag;
}

using std::cout;
using std::cerr;
using std::endl;
using std::string;

//template <typename Func>
void get_permutations(std::string& numbers) {
    std::sort(numbers.begin(), numbers.end());
    do {
        unsigned check_me = boost::lexical_cast<unsigned>(numbers);
        if ((check_me % 2) == 1) {
            // todo : threaded pool
#if 0
            aks_test aks(check_me);
            if (aks()) {
#else
            if(isPrimeAKSFaster(NTL::to_ZZ(check_me))){
#endif
                cout << check_me << endl;
            }

        }
    }
    while (std::next_permutation(numbers.begin(), numbers.end()));
}

int main(int argc, char* argv[]) {


    if (argc != 2) {
        cout << "Usage: primetest <numbers>" << endl;
    }

#if 1
    try {
        string strnums(argv[1]);
        //get_permutations<aks_test>(strnums);
        double sttime;
        sttime = NTL::GetTime();

        get_permutations(strnums);
        cout << "The Time taken in checking primality is " << NTL::GetTime() - sttime << endl;
        cout << endl;
    }
    catch (boost::bad_lexical_cast& e) {
        cerr << e.what() << "(not a number?)" << endl;
    }
#else
    string strnums(argv[1]);
    unsigned check_me = boost::lexical_cast<unsigned>(strnums);
    NTL::INIT_VAL_STRUCT t = {};
    ntl_bigint bcheckme(t, check_me);
    if (aks_test(bcheckme)) {
        cout << check_me << " is prime" << endl;
    }
    else {
        cout << check_me << " is composite" << endl;
    }
#endif

    return 0;
}

