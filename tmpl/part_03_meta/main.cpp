#include <iostream>
#include "power.h"
#include "sqrt.h"
#include "dot_product.h"

using std::cout;
using std::endl;

// вычисление степени целого числа с помощью метапрограммирования
void show_power(){
	// это значение вычислено на этапе компиляции
	// (потенциально можно применять для расчета констант, 
	// которые компилятор не в состоянии соптимизировать)
	int pwr = Pow<3, 5>::result;
	cout << "3^5 = " << pwr << endl;

	pwr = Pow<2, 4>::result;
	cout << "2^4 = " << pwr << endl;
}

// вычисление целочисленного квадратного корня с помощью метапрограммирования
void show_sqrt(){
	// это значение вычислено на этапе компиляции
	int sq = Sqrt<16>::result;
	cout << "sqrt(16) = " << sq << endl;

	sq = Sqrt<42>::result;
	cout << "sqrt(42) = " << sq << endl;
}

// Развертка коротких циклов с помощью метапрограммирования - 
// нахождение скалярного произведения массивов
void show_dot_product(){

	int a[] = {3, 4, 1};
	int b[] = {5, 6, 7};
	// требуется явно указать размерность массива
	int pr = dot_product<3, int>(a, b);
	cout << "product = " << pr << endl;

	double c[3] = {3.0, 5.4, 0.1};
	double d[3] = {5.1, 0.6, 7.5};
	// требуется явно указать размерность массива
	double dpr = dot_product<3, double>(c, d);
	cout << "product = " << dpr << endl;
}

int main(){

	show_power();
	show_sqrt();
	show_dot_product();

	return 0;
}