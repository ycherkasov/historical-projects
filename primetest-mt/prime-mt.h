#ifndef PRIMETEST_MT_NTL_H
#define PRIMETEST_MT_NTL_H

typedef unsigned bigint_t;
typedef unsigned polynome_t;

class aks_test_mt{
public:
    
    aks_test_mt(bigint_t n);
    
    bool is_prime();
    
protected:
    
    bool is_power();
    
    bigint_t find_r();
    
    void modulo_exponents(polynome_t &p, const bigint_t &r)
    
    polynome_t build_polynome(bigint_t r, polynome_t p);
    
private:
    const bigint_t _number;
    long _log2n;
};

#endif //PRIMETEST_MT_NTL_H
