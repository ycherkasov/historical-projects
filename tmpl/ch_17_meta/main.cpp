#include <iostream>
#include "power.h"
#include "sqrt.h"
#include "dot_product.h"

using std::cout;
using std::endl;

// ���������� ������� ������ ����� � ������� ��������������������
void show_power(){
	// ��� �������� ��������� �� ����� ����������
	// (������������ ����� ��������� ��� ������� ��������, 
	// ������� ���������� �� � ��������� ���������������)
	int pwr = Pow<3, 5>::result;
	cout << "3^5 = " << pwr << endl;

	pwr = Pow<2, 4>::result;
	cout << "2^4 = " << pwr << endl;
}

// ���������� �������������� ����������� ����� � ������� ��������������������
void show_sqrt(){
	// ��� �������� ��������� �� ����� ����������
	int sq = Sqrt<16>::result;
	cout << "sqrt(16) = " << sq << endl;

	sq = Sqrt<42>::result;
	cout << "sqrt(42) = " << sq << endl;
}

// ��������� �������� ������ � ������� �������������������� - 
// ���������� ���������� ������������ ��������
void show_dot_product(){

	int a[] = {3, 4, 1};
	int b[] = {5, 6, 7};
	// ��������� ���� ������� ����������� �������
	int pr = dot_product<3, int>(a, b);
	cout << "product = " << pr << endl;

	double c[3] = {3.0, 5.4, 0.1};
	double d[3] = {5.1, 0.6, 7.5};
	// ��������� ���� ������� ����������� �������
	double dpr = dot_product<3, double>(c, d);
	cout << "product = " << dpr << endl;
}

int main(){

	show_power();
	show_sqrt();
	show_dot_product();

	return 0;
}