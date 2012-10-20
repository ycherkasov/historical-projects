#ifndef PRIMETEST_MT_NTL_H
#define PRIMETEST_MT_NTL_H

#include <string>
#include <stdexcept>
extern "C"{
#include "fmpz.h"
}

class uint_big{
public:
    uint_big(ulong n = 0){
        fmpz_init(x);
        fmpz_set_ui(x, n);
    }

    uint_big(const char* str){
        fmpz_init(x);
        fmpz_set_str(x, const_cast<char*>(str), 10);
    }
    
    uint_big(const uint_big& rhs){
        fmpz_init(x);
        fmpz_set(x, rhs.x);
    }
    
    uint_big& operator=(const uint_big& rhs){
        if(this != &rhs){
            fmpz_set(x, rhs.x);
        }
        return *this;
    }
    
    uint_big& operator=(ulong n){
        fmpz_set_ui(x, n);
        return *this;
    }
    
    void swap(uint_big& rhs){
        fmpz_swap(x, rhs.x);
    }
    
    ~uint_big(){
        fmpz_clear(x);
    }
    
    // random generation: todo
    
    // conversions
    ulong get_ulong() const {
        return fmpz_get_ui(x);
    }
    
    std::string get_string() const {
        char* buf = fmpz_get_str( 0 ,10 , x );
        if(buf){
            std::string ret(buf);
            free(buf);
            return ret;
        }
        else{
            throw std::runtime_error("unable to converl big uint to string");
        }
    }
    
    // properties
    size_t num_digits() const {
        return fmpz_sizeinbase(x, 10);
    }

    size_t num_bits() const {
        return fmpz_bits(x);
    }
    
    bool is_even() const {
        return fmpz_is_even(x);
    }
    
    bool is_odd() const {
        return fmpz_is_odd(x);
    }
    
        
    bool is_one() const {
        return fmpz_is_one(x);
    }
    
    bool is_zero() const {
        return fmpz_is_zero(x);
    }
    
    // operators
    bool operator > (const uint_big& other) const {
        return (fmpz_cmp(x, other.x) > 0);
    }
  
    bool operator <(const uint_big& other) const {
        return (fmpz_cmp(x, other.x) < 0);
    }
    
    bool operator >= (const uint_big& other) const {
        return ((fmpz_equal(x, other.x)) 
                || (fmpz_cmp(x, other.x) > 0));
    }
  
    bool operator <=(const uint_big& other) const {
        return ((fmpz_equal(x, other.x)) 
                || (fmpz_cmp(x, other.x) < 0));
    }
    
    bool operator ==(const uint_big& other) const {
        return (fmpz_equal(x, other.x) == 1);
    }
    
    bool operator != (const uint_big& other) const {
        return !(*this == other);
    }
    
    uint_big& operator ++ () {
        //x += 1;
        fmpz_add_ui(x, x, 1);
        return *this;
    }
    
    uint_big operator ++ (int) {
        uint_big tmp(*this);
        //x += 1;
        fmpz_add_ui(x, x, 1);
        return tmp;
    }

    friend uint_big operator+(const uint_big& a, const uint_big& b){
        fmpz_t ret;
        fmpz_add(ret, a.x, b.x);
        return uint_big(ret);
    }

    
    friend uint_big operator-(const uint_big& a, const uint_big& b){
        fmpz_t ret;
        fmpz_sub(ret, a.x, b.x);
        return uint_big(ret);
    }
    
    friend uint_big operator*(const uint_big& a, const uint_big& b){
        fmpz_t ret;
        fmpz_mul(ret, a.x, b.x);
        return uint_big(ret);
    }


    friend uint_big operator/(const uint_big& a, const uint_big& b){
        fmpz_t ret;
        fmpz_init(ret);
        fmpz_fdiv_q(ret, a.x, b.x);
        return uint_big(ret);
    }
    
        
    friend uint_big operator%(const uint_big& a, const uint_big& b){
        fmpz_t ret;
        fmpz_init(ret);
        fmpz_mod(ret, a.x, b.x);
        return uint_big(ret);
    }
    
    void power(ulong p){
        fmpz_t tmp;
        fmpz_init(tmp);
        fmpz_set(tmp, x);
        fmpz_pow_ui(x, tmp, p);
    }
    
        
    void power_mod(ulong p, const uint_big& mod){
        fmpz_t tmp;
        fmpz_init(tmp);
        fmpz_set(tmp, x);
        fmpz_powm_ui(x, tmp, p, mod.x);
    }
    
        
    void power_mod(const uint_big& p, const uint_big& mod){
        fmpz_t tmp;
        fmpz_init(tmp);
        fmpz_set(tmp, x);
        fmpz_powm(x, tmp, p.x, mod.x);
    }
    
        
    uint_big gcd(const uint_big& n){
        fmpz_t tmp;
        fmpz_init(tmp);
        fmpz_set(tmp, x);
        fmpz_gcd(tmp, x, n.x);
        return tmp;
    }


private:
        
    // constructor with direct access to fmpz representation
    // for binary operators only
    uint_big(const fmpz_t rhs){
        fmpz_init(x);
        fmpz_set(x, rhs);
    }
public:
    fmpz_t x;
};






class aks_test_mt{
public:
    
    aks_test_mt(const uint_big& n)
        : _number(n)
        , _log2n(n.num_bits()){}
    
    //bool is_prime();
    
public:
//protected:

    bool is_power(){
        uint_big upper_bound(_number);
        uint_big lower_bound(1);
        uint_big temp, the_power;

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

    uint_big find_r(){
        uint_big q, r;
        uint_big zero;
        //uint_big j;
        long j;
        uint_big mods, nmodq, jmodq;
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
        uint_big a;
        uint_big gcd;
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
    const uint_big _number;
    long _log2n;
};


#endif //PRIMETEST_MT_NTL_H
