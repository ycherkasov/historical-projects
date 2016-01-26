/* 
 * File:   polynome_big.h
 * Author: straight
 *
 * Created on October 20, 2012, 6:28 PM
 */

#ifndef POLYNOME_BIG_H
#define	POLYNOME_BIG_H

#include <string>
#include <stdexcept>

extern "C"{
#include "flint.h"
#include "fmpz_poly.h"
}

#include "uint-big.h"



class polynome_big{
public:
    
    polynome_big(){
        fmpz_poly_init(x);
    }

    polynome_big(const polynome_big& rhs){
        fmpz_poly_init(x);
        fmpz_poly_set(x, rhs.x);
    }
#if 0
    // does not work
    // todo: write flint-devel?
    polynome_big(const char* poly_str){
        fmpz_poly_init(x);
        int ret = fmpz_poly_set_str(x, poly_str);
        if(ret != 0){
            throw std::runtime_error("Unable to convert string to polynome");
        }
    }
#endif

    polynome_big& operator=(const polynome_big& rhs){
        if(this != &rhs){
            fmpz_poly_set(x, rhs.x);
        }
        return (*this);
    }

    ~polynome_big(){
        fmpz_poly_clear(x);
    }

    // parameters
    
    long lenght() const {
        return fmpz_poly_length(x);
    }
    
    long degree() const {
        return fmpz_poly_degree(x);
    }
    
    long max_bits() const {
        return fmpz_poly_max_bits(x);
    }
    
    std::string get_string() const {

        char* c_poly_str = fmpz_poly_get_str_pretty(x, "x");
        
        if(c_poly_str){
            std::string ret(c_poly_str);
            free(c_poly_str);
            return ret;
        }
        else throw std::runtime_error("Unable to convert polynome to string");
    }
    
    // settinng coefficients
    void set_coeff(long n, long m){
        fmpz_poly_set_coeff_si(x, n, m);
    }

    void set_coeff(long n, const big_int& m){
        fmpz_poly_set_coeff_fmpz(x, n, m.x);
    }
    
    // comparasions
    
    bool operator == (const polynome_big& other) const {
        return fmpz_poly_equal(x, other.x) ? true : false;
    }


    bool operator != (const polynome_big& other) const {
        return !((*this) == other);
    }
    
    bool is_zero() const {
        return fmpz_poly_is_zero(x) ? true : false;
    }
    
    bool is_one() const {
        return fmpz_poly_is_one(x) ? true : false;
    }

    // + -
    
    friend polynome_big operator+(const polynome_big& a, const polynome_big& b){
        fmpz_poly_t ret;
        fmpz_poly_init(ret);
        fmpz_poly_add(ret, a.x, b.x);
        return polynome_big(ret);
    }

    friend polynome_big operator-(const polynome_big& a, const polynome_big& b){
        fmpz_poly_t ret;
        fmpz_poly_init(ret);
        fmpz_poly_sub(ret, a.x, b.x);
        return polynome_big(ret);
    }
    
    friend polynome_big operator*(const polynome_big& a, const polynome_big& b){
        fmpz_poly_t ret;
        fmpz_poly_init(ret);
        fmpz_poly_mul(ret, a.x, b.x);
        return polynome_big(ret);
    }
    
    // Scalar and other modifying operation
    
    polynome_big& scalar_mul(const big_int& n){
        fmpz_poly_scalar_mul_fmpz(x, x, n.x);
        return (*this);
    }

    polynome_big& scalar_mul(long n){
        fmpz_poly_scalar_mul_si(x, x, n);
        return (*this);
    }

    static polynome_big scalar_mul(const polynome_big& p, const big_int& n){
        return polynome_big(p).scalar_mul(n);
    }

    static polynome_big scalar_mul(const polynome_big& p, long n){
        return polynome_big(p).scalar_mul(n);
    }

    polynome_big& power(ulong e){
        fmpz_poly_pow(x, x, e);
        return (*this);
    }

    static polynome_big power(const polynome_big& p, ulong e){
        return polynome_big(p).power(e);
    }


private:    

    polynome_big(fmpz_poly_t rhs){
        fmpz_poly_init(x);
        fmpz_poly_set(x, rhs);
    }
private:
    fmpz_poly_t x;
};

#endif	/* POLYNOME_BIG_H */

