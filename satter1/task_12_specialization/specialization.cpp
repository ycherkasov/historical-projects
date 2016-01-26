#include <iostream>
#include <complex>

using namespace std;

// Task 1.17 From Satter 1
// Template specialization and overload

// 1. general case
template <typename T1, typename T2>
void foo(T1, T2){
    cout << "1: void foo(T1, T2)" << endl;
}

// 2. One param
template <typename T>
void foo(T){
    cout << "2: void foo(T)" << endl;
}

// 3. Two equivalent params
template <typename T>
void foo(T, T){
    cout << "3: void foo(T, T)" << endl;
}

// 4. One pointer param
template <typename T>
void foo(T*){
    cout << "4: void foo(T*)" << endl;
}

// 5. First is pointer, second is value
template <typename T>
void foo(T*, T){
    cout << "5: void foo(T*, T)" << endl;
}

// 6. First is value, second is pointer
template <typename T>
void foo(T, T*){
    cout << "6: void foo(T, T*)" << endl;
}

// 7. First is int, second is pointer
template <typename T>
void foo(int, T*){
    cout << "7: void foo(int, T*)" << endl;
}

// 8. Int, template function
template <>
void foo<int>(int){
    cout << "8: void foo<int>(int)" << endl;
}

// 9. Int, double - plain function
void foo(int, double){
    cout << "9: void foo(int, double)" << endl;
}

// 10. Int - plain function
void foo(int){
    cout << "10: void foo(int)" << endl;
}

int main(){

    int i = 0;
    double d = 0.;
    float f = 0.;
    complex<double> c(0., 0.);

    foo(i);     // 10
    foo<int>(i);// 8
    foo(i, i);  // 3

    foo(c);     // 2
    foo(i, f);  // 1
    foo(i, d);  // 9

    foo(c, &c); // 6
    foo(i, &d); // 7
    foo(&d, d); // 5
    foo(&d);    // 4

    foo(d, &i); // 1
    foo(&i, &i);// 3

}

