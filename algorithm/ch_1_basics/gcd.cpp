#include <cmath>
#include <iostream>
#include "gcd.h"

using namespace std;
/*
Levitin book 1.1
Algorithm basics
Greater common divisor exercise
*/

/// todo: extended Euckid

/// Greater common divisor implementation
/// return : Greater common divisor for 'm' and 'n'
/// gcd(m, n) = gcd(n, m mod n)
/*
Prove:
http://e-maxx.ru/algo/euclid_algorithm
*/
// recursive form
int gcd (int n, int m) {
    int a = std::max(n, m);
    int b = std::min(n, m);
    if (b == 0)
        return a;
    else
        return gcd (b, a % b);
}

// non-recursive form
int gcd2(int n, int m) {
    int a = std::max(n, m);
    int b = std::min(n, m);
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

/// Exercise 1.8
/// Euclid in his tractat used division instead of mod
/// operation. Let's implement this part of algorithm.
int euclid_gcd(int n, int m){
    int a = std::max(n, m);
    int b = std::min(n, m);
    while(a != b) {
        (a > b ) ? a-=b : b-=a;
        std::swap(a,b);
    }
    return a;
}

// Вычисление наименьшего общего кратного (least common multiplier, lcm)
int lcm (int a, int b) {
    return a / gcd (a, b) * b;
}


// Соотношение Безу
// В теории чисел соотноше́ние Безу́ — соотношение между парой целых чисел и их наибольшим общим делителем
/*
Пусть a, b — целые числа, хотя бы одно из которых не нуль. 
Тогда существуют такие целые числа x, y, что выполняется соотношение:
НОД(a,b) = x·a + y·b.

Другими словами, наибольший общий делитель чисел a, b можно всегда представить 
как линейную комбинацию a и b с целыми коэффициентами.
*/

struct euclid_triple{
    int _d, _x, _y;
    euclid_triple(int d, int x, int y):_d(d),_x(x),_y(y){}
    euclid_triple& operator=(const euclid_triple& rhs){
        if(&rhs != this){
            _d = rhs._d;
            _x = rhs._x;
            _y = rhs._y;
        }
        return *this;
    }
};

euclid_triple extended_euclid(int a, int b){
    if(b == 0) 
        return euclid_triple(a,1,0);
    euclid_triple ret(0,0,0);
    euclid_triple triple = extended_euclid( b, a % b );
    int fl = a/b;
    ret = euclid_triple(triple._d, triple._y, triple._x - fl * triple._y);
    return ret;

}

int show_gcd()
{
    cout << "gcd(60, 24) = " << gcd(60, 24) << endl;
    cout << "gcd2(60, 24) = " << gcd2(60, 24) << endl;
    cout << "euclid_gcd(60, 24) = " << euclid_gcd(60, 24) << endl;
    cout << endl;
    cout << "gcd(2, 18) = " << gcd(2, 18) << endl;
    cout << "gcd2(2, 18) = " << gcd2(2, 18) << endl;
    cout << "euclid_gcd(2, 18) = " << euclid_gcd(2, 18) << endl;
    cout << endl;
    cout << "gcd(1245, 5689) = " << gcd(1245, 5689) << endl;
    cout << "gcd2(1245, 5689) = " << gcd2(1245, 5689) << endl;
    cout << "euclid_gcd(1245, 5689) = " << euclid_gcd(1245, 5689) << endl;
    cout << endl;
    cout << "gcd(1838, 7855) = " << gcd(1838, 7855) << endl;
    cout << "gcd2(1838, 7855) = " << gcd2(1838, 7855) << endl;
    cout << "euclid_gcd(1838, 7855) = " << euclid_gcd(1838, 7855) << endl;
    cout << endl;
    cout << "gcd(1265, 15540) = " << gcd(1265, 15540) << endl;
    cout << "gcd2(1265, 15540) = " << gcd2(1265, 15540) << endl;
    cout << "euclid_gcd(1265, 15540) = " << euclid_gcd(1265, 15540) << endl;
    cout << endl;
    int x = 0;
    int y = 0;
    euclid_triple d = extended_euclid(60, 24);
    cout << "60 * " << d._x << " + 24 * " << d._y << " = " << d._d << endl;
    d = extended_euclid(30, 12);
    cout << "30 * " << d._x << " + 12 * " << d._y << " = " << d._d << endl;
    d = extended_euclid(99, 78);
    cout << "99 * " << d._x << " + 78 * " << d._y << " = " << d._d << endl;

    return 0;
}