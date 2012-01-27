#include <cmath>
#include <iostream>

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

int extended_euclid(int a, int b, int& x_out, int& y_out){
    int x = 0;
    int y = 0;

    if(b == 0){
        return a;
    }

    int x2 = 1, x1 = 0;
    int y2 = 0, y1 = 1;
    while(b > 0){
        int q = a % b;
        int r = a - q*b;
        x = x2 - q*x1;
        y = y2 - q*y1;
        a = b; b = r; x2 = x1; x1 = x; y2 = y1; y1 = y;
    }
    x_out = x2;
    y_out = y2;
    return a;
}





int main()
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
    //int d = extended_euclid(60, 24, x ,y);
    //cout << "60 * " << x << " + 24 * " << y << " = " << d << endl;
    //d = extended_euclid(30, 12, x ,y);
    //cout << "12 * " << x << " + 30 * " << y << " = " << d << endl;

    return 0;
}