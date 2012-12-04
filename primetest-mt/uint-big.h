/* 
 * File:   uint_big.h
 * Author: straight
 *
 * Created on October 20, 2012, 6:10 PM
 */

#ifndef UINT_BIG_H
#define	UINT_BIG_H

extern "C"{
#include "fmpz.h"
}


class big_int{
public:
    

    /////////////////////////////////////////////////////////////////////////////////////
    // construction and assignment
    
    /** @brief Big integer from long value is initialized */
    /** @param n long value converted to bigint */
    big_int(long n = 0){
        fmpz_init(x);
        fmpz_set_si(x, n);
    }

    /** @brief Big integer from string representation is initialized 
     *  (could be used if long is not enough to represent value) */
    /** @param str string representation of bigint value  */
    big_int(const char* str){
        fmpz_set_str(x, const_cast<char*>(str), 10);
    }
    
    /** @brief Copy from another value */
    big_int(const big_int& rhs){
        fmpz_set(x, rhs.x);
    }
    
    /** @brief Assign from another bigint value */
    big_int& operator=(const big_int& rhs){
        if(this != &rhs){
            fmpz_set(x, rhs.x);
        }
        return *this;
    }
    
    /** @brief Assign from another long value */
    big_int& operator=(long n){
        fmpz_set_si(x, n);
        return *this;
    }
    
    /** @brief Quick swap without memory reallocation */
    void swap(big_int& rhs){
        fmpz_swap(x, rhs.x);
    }
    
    /** @brief Destructor is NOT virtual. This class is not intended for inheritance */
    ~big_int(){
        fmpz_clear(x);
    }

    /////////////////////////////////////////////////////////////////////////////////////
    // random generation
    
    /** @brief Generate random bigint number in range [from; to]*/
    void random_init(const big_int& from, const big_int& to) {
        if(from >= to){
            throw std::runtime_error("random_init: 'from' value must be less than 'to'");
        }
        flint_rand_t rndstate;
        flint_randinit(rndstate);
        big_int tmp(to - from);
        fmpz_randm(x, rndstate, tmp.x);
        fmpz_add(x, x, from.x);
        flint_randclear(rndstate);
    }

    /////////////////////////////////////////////////////////////////////////////////////
    // conversions
    
    /** @brief Convert big_int to long 
     * If current value does not fit to a long, result is undefined */
    long get_long() const {
        return fmpz_get_si(x);
    }
    
    /** @brief Convert big_int to string */
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
    
    /////////////////////////////////////////////////////////////////////////////////////
    // Properties
    
    
#if 0
    // return 4 on 999 for some reasons (write to flint-devel or test on newest versions)
    /** @brief Number of digits in big_int by base 10 */
    size_t num_digits() const {
        return fmpz_sizeinbase(x, 10);
    }
#endif

    /** @brief Number of bits requires to store big_int */
    size_t num_bits() const {
        return fmpz_bits(x);
    }
    
    /** @brief Is curent big_int is even */
    bool is_even() const {
        return fmpz_is_even(x);
    }
    
    /** @brief Is curent big_int is odd */
    bool is_odd() const {
        return fmpz_is_odd(x);
    }
    
    /** @brief Efficient compare == 1 */    
    bool is_one() const {
        return fmpz_is_one(x);
    }
    
    /** @brief Efficient compare == 0 */    
    bool is_zero() const {
        return fmpz_is_zero(x);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////
    // Comtaration operators
    
    /** @brief operator > */
    bool operator > (const big_int& other) const {
        return (fmpz_cmp(x, other.x) > 0);
    }
  
    /** @brief operator < */
    bool operator < (const big_int& other) const {
        return (fmpz_cmp(x, other.x) < 0);
    }
    
    /** @brief operator >= */
    bool operator >= (const big_int& other) const {
        return ((fmpz_equal(x, other.x)) 
                || (fmpz_cmp(x, other.x) > 0));
    }
  
    /** @brief operator <= */
    bool operator <= (const big_int& other) const {
        return ((fmpz_equal(x, other.x)) 
                || (fmpz_cmp(x, other.x) < 0));
    }
    
    /** @brief operator == */
    bool operator ==(const big_int& other) const {
        return (fmpz_equal(x, other.x) == 1);
    }
    
    /** @brief operator != */
    bool operator != (const big_int& other) const {
        return !(*this == other);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////
    // Unary operations - implemented as operators

    /** @brief Prefix operator ++ */
    big_int& operator ++ () {
        //x += 1;
        fmpz_add_ui(x, x, 1);
        return *this;
    }
    
    /** @brief Postfix operator ++ */
    big_int operator ++ (int) {
        big_int tmp(*this);
        //x += 1;
        fmpz_add_ui(x, x, 1);
        return tmp;
    }

    /** @brief Prefix operator -- */
    big_int& operator -- () {
        //x += 1;
        fmpz_sub_ui(x, x, 1);
        return *this;
    }
    
    /** @brief Postfix operator -- */
    big_int operator -- (int) {
        big_int tmp(*this);
        //x += 1;
        fmpz_sub_ui(x, x, 1);
        return tmp;
    }

    /** @brief Unary operator -- */
    big_int operator - () const {
        big_int tmp(*this);
        fmpz_neg(tmp.x, x);
        return tmp;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////
    // Commutative operations - implemented as friend functions
    
    /** @brief Operator + */
    friend big_int operator+(const big_int& a, const big_int& b){
        fmpz_t ret;
        fmpz_add(ret, a.x, b.x);
        return big_int(ret);
    }

    /** @brief Operator - */
    friend big_int operator-(const big_int& a, const big_int& b){
        fmpz_t ret;
        fmpz_sub(ret, a.x, b.x);
        return big_int(ret);
    }
    
    /** @brief Operator * */
    friend big_int operator*(const big_int& a, const big_int& b){
        fmpz_t ret;
        fmpz_mul(ret, a.x, b.x);
        return big_int(ret);
    }

    /** @brief Operator / */
    friend big_int operator/(const big_int& a, const big_int& b){
        fmpz_t ret;
        fmpz_init(ret);
        fmpz_fdiv_q(ret, a.x, b.x);
        return big_int(ret);
    }
    
    /** @brief Operator % */
    friend big_int operator%(const big_int& a, const big_int& b){
        fmpz_t ret;
        fmpz_init(ret);
        fmpz_mod(ret, a.x, b.x);
        return big_int(ret);
    }
    

    /////////////////////////////////////////////////////////////////////////////////////
    // Modifying operations - implemented as methods

    /** @brief Raise big_int to power p */
    big_int& power(long p){
        fmpz_t tmp;
        fmpz_init(tmp);
        fmpz_set(tmp, x);
        fmpz_pow_ui(x, tmp, p);
        return (*this);
    }
    
    /** @brief Static version: Raise big_int a to power p */
    static big_int power(const big_int& a, long p){
        return big_int(a).power(p);
    }
    
    /** @brief Raise big_int to power p by module mod */
    big_int& power_mod(ulong p, const big_int& mod){
        fmpz_t tmp;
        fmpz_init(tmp);
        fmpz_set(tmp, x);
        fmpz_powm_ui(x, tmp, p, mod.x);
        return (*this);
    }

    /** @brief Static version: Raise big_int a to power p by module mod */
    static big_int power_mod(const big_int& a, long p, const big_int& mod){
        return big_int(a).power_mod(p, mod);
    }

    /** @brief Raise big_int to power p by module mod */
    big_int& power_mod(const big_int& p, const big_int& mod){
        fmpz_t tmp;
        fmpz_init(tmp);
        fmpz_set(tmp, x);
        fmpz_powm(x, tmp, p.x, mod.x);
        return (*this);
    }
    
    /** @brief Static version: Raise big_int a to power p by module mod */
    static big_int power_mod(const big_int& a, const big_int& p, const big_int& mod){
        return big_int(a).power_mod(p, mod);
    }

    /** @brief Take quare root of big_int */
    big_int& sqr_root(){
        fmpz_t tmp;
        fmpz_init(tmp);
        fmpz_set(tmp, x);
        fmpz_sqrt(x, tmp);
        return (*this);
    }

    /** @brief Static version: Take quare root of big_int */
    static big_int sqr_root(const big_int& a){
        return big_int(a).sqr_root();
    }

    /** @brief Find greatest common divisor of big_int and n */
    big_int gcd(const big_int& n) const {
        fmpz_t tmp;
        fmpz_init(tmp);
        fmpz_set(tmp, x);
        fmpz_gcd(tmp, x, n.x);
        return tmp;
    }
    
        
    /** @brief Static version: Find greatest common divisor of big_int a and b */
    static big_int gcd(const big_int& a, const big_int& b){
        return big_int(a).gcd(b);
    }

    
private:
        
    /// constructor with direct access to fmpz representation
    /// for binary operators only which are friend functions
    big_int(const fmpz_t rhs){
        fmpz_init(x);
        fmpz_set(x, rhs);
    }
public:
    
    /// FLINT representation of big integer
    fmpz_t x;
};


#endif	/* UINT_BIG_H */

