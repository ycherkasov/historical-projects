package com.angelusworld.aks.algorithm;

import com.angelusworld.aks.AKSView;
import java.math.BigInteger;
import nuim.cs.crypto.polynomial.PolynomialException;
import nuim.cs.crypto.polynomial.big.BigPolynomial;
import nuim.cs.crypto.polynomial.big.field.BigFieldPolynomial;

/**
 * AKS algorithm's implementation. The application's core.
 * @author Valerio Capozio
 * @author <a href="http://www.angelusworld.com">http://www.angelusworld.com</a>
 * @author <a href="mailto:valeriocapozio@angelusworld.com">valeriocapozio@angelusworld.com</a>
 * @version 1.0
 */
public class AKSAlgorithm implements Runnable {

    private BigInteger n;

    public AKSAlgorithm(BigInteger aBigInteger) {
        this.n = aBigInteger;
    }

    /**
     * Calculates the pow between two numbers, base and exponent, both BigInteger.
     * @param base. 
     * @param exponent
     * @return base^exponent
     */
    public BigInteger pow(BigInteger base, BigInteger exponent) {
        BigInteger p = BigInteger.ONE;
        while (exponent.compareTo(new BigInteger("0")) > 0) { // b^e * p = b0 ^ e0
            if (exponent.remainder(new BigInteger("2")).compareTo(BigInteger.ONE) == 0) {
                p = p.multiply(base);
            }
            base = base.multiply(base);
            exponent = exponent.divide(new BigInteger("2"));
        }
        return p;
    }

    /**
     * Check if n is two pow. n=two^b
     * @param n 
     * @return true if n is two pow, false otherwise.
     */
    public boolean isPow(BigInteger n) {
        BigInteger upperBound = n;
        BigInteger lowerBound = BigInteger.ONE;
        BigInteger temp;
        for (BigInteger i = BigInteger.ONE; (i.compareTo(new BigInteger(new Integer((n.bitLength())).toString())) < 0); i = i.add(BigInteger.ONE)) {
            while ((upperBound.subtract(lowerBound)).compareTo(BigInteger.ONE) > 0) {
                temp = ((upperBound.add(lowerBound)).divide(new BigInteger("2")));
                if ((pow(temp, i.add(BigInteger.ONE))).compareTo(n) == 0) {
                    AKSView.appendText("The number " + n + " is not prime. " + n + " = " + temp + "^" + i.add(BigInteger.ONE) + "\n");
                    return true;
                }
                if ((pow(temp, i.add(BigInteger.ONE))).compareTo(n) > 0) {
                    upperBound = temp;
                }
                if ((pow(temp, i.add(BigInteger.ONE))).compareTo(n) < 0) {
                    lowerBound = temp;
                }
            }
        }
        AKSView.appendText("The number " + n + " is not pow. \n");
        return false;
    }

    /**
     * Check if the number r is prime.
     * @param r BigInteger number to be chek
     * @return true if r is prime, false otherwise.
     */
    public boolean isPrime(BigInteger r) {
        BigInteger two = new BigInteger("2");
        while ((two.multiply(two)).compareTo(r) <= 0) {
            if (r.remainder(two).compareTo(BigInteger.ZERO) == 0) {
                return false;
            }
            two = two.add(BigInteger.ONE);
        }
        return true;
    }

    /**
     * Find the right value of the parameter r
     * @param n BigInteger
     * @return BigInteger the value of the parameter r
     */
    public BigInteger findR(BigInteger n) {
        BigInteger r = new BigInteger("2");
        BigInteger lgn = new BigInteger((new Integer((n.subtract(BigInteger.ONE)).bitLength())).toString());
        BigInteger s1 = lgn.multiply(new BigInteger("4")).multiply(lgn);
        while ((r.compareTo(n) < 0)) {
            if (n.remainder(r).compareTo(BigInteger.ZERO) == 0) {
                AKSView.appendText("The number " + n + " is not prime. " + r + " | " + n + "\n");
                return BigInteger.ZERO;  // r|n
            }
            if (isPrime(r)) {
                for (BigInteger i = BigInteger.ONE; (i.compareTo(s1) <= 0); i = i.add(BigInteger.ONE)) {
                    if ((n.modPow(i, r).compareTo(BigInteger.ONE) == 0)) {// if(n^i==1 mod r) test next r
                        break;
                    }
                    if ((i.add(BigInteger.ONE).compareTo(s1) == 0)) { //found r
                        return r;
                    }
                }
            }
            r = r.add(BigInteger.ONE);
        }
        AKSView.appendText("There are not dividers less or equal " + r + "\n");
        return r;
    }

    /**
     * Executes algorithm
     */
    public void run() {
        while (true) {
            if (n.compareTo(BigInteger.ONE) == 0) {
                AKSView.appendText("1 is prime.\n");
                AKSView.stopBusyIcon();
                return;
            }
            if (isPow(n)) {
                AKSView.stopBusyIcon();
                return;
            }
            BigInteger r = findR(n);
            if (r.compareTo(BigInteger.ZERO) == 0) {
                AKSView.stopBusyIcon();
                return;
            }
            if (r.compareTo(n) == 0) {
                AKSView.appendText("The number " + n + " is prime! \n");
                AKSView.stopBusyIcon();
                return;
            } else {
                AKSView.appendText(n + " > " + r + "\n");
                BigInteger lgn = new BigInteger((new Integer((n.subtract(BigInteger.ONE)).bitLength())).toString());
                BigInteger sL = (new BigInteger(new Integer((new Double(Math.sqrt(r.doubleValue()))).intValue()).toString())).multiply(lgn).multiply(new BigInteger("2"));
                BigFieldPolynomial modulus = new BigFieldPolynomial(new BigPolynomial("x^" + r.toString() + "-1"), n);
                for (BigInteger a = new BigInteger("1"); (a.compareTo(sL) < 0); a = a.add(BigInteger.ONE)) {
                    //x+a mod n
                    BigFieldPolynomial u = new BigFieldPolynomial(new BigPolynomial("x+" + a.toString()), n);
                    //x^n+a mod n
                    BigFieldPolynomial v = new BigFieldPolynomial(new BigPolynomial("x^" + n.toString() + "+" + a.toString()), n);
                    try {
                        //(x+a)^n mod(x^r-1,n)
                        BigFieldPolynomial sx = new BigFieldPolynomial(u.modPow(n, modulus), n);
                        //(x^n+a) mod(x^r-1,n)
                        BigFieldPolynomial dx = new BigFieldPolynomial(v.mod(modulus).modCoefficient(n), n);
                        AKSView.appendText(sx + " = " + dx + "\n");
                        if (!(sx.equals(dx))) {
                            AKSView.appendText("Polynomials do not match. The number " + n + " is not prime.\n");
                            AKSView.stopBusyIcon();
                            return;
                        }
                    } catch (PolynomialException e) {
                        System.out.println(e.getMessage());
                    }
                }
                AKSView.appendText("All polynomial congruences were successfully completed. The number " + n + " is prime.\n");
                AKSView.stopBusyIcon();
                break;
            }
        }
    }
}
