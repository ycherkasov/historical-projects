#include <iostream>
#include "streams.h"

using namespace std;

// ������������� ����� ����� ����� ���������
// - �����
// - ������
// - ������������

// �����
void stream_flags(){

	// ��������� ������� ������ ������
	ios::fmtflags flags = cout.flags();

	// ����� �������� ��� ������ ���������� ��������
	// flags()  - ������ =
	// setf() - ������ OR
	
	// ������������ �����
	{
		// ios::adjustfield - ��� ������ � ������� ������ ������������
		cout.setf(ios::left, ios::adjustfield);
		cout << "left: " << 10 << endl;
		cout.unsetf(ios::left);
	}

	// ������������ ������
	{
		// ios::adjustfield - ��� ������ � ������� ������ ������������
		cout.setf(ios::right, ios::adjustfield);
		cout << "right: " << 20 << endl;
		cout.unsetf(ios::right);
	}

	// ���� ������, ����� �����
	{
		// ios::adjustfield - ��� ������ � ������� ������ ������������
		cout.setf(ios::internal, ios::adjustfield);
		cout << "internal: " << -20 << endl;
		cout.unsetf(ios::internal);
	}

	// hex + �������
	{
		// ios::basefield - ��� ������ � ������� ������ ������� ���������
		cout.setf(ios::hex|ios::showbase, ios::basefield);
		cout << "hex|showbase: " << 0xffff << endl;
		cout.unsetf(ios::hex|ios::showbase);
	}

	// ����� �������� � ������
	{
		cout.setf(ios::fixed);
		cout << "fixed: " << 0.0000001 << endl;
		cout.unsetf(ios::fixed);
	}

	// ����� �������� � ���������������� ����
	{
		cout.setf(ios::scientific);
		cout << "scientific: " << 0.0000001 << endl;
		cout.unsetf(ios::scientific);
	}

	// ����� bool �������
	{
		cout.setf(ios::boolalpha);
		cout << "boolalpha: " << true << endl;
		cout.unsetf(ios::boolalpha);
	}

	// ����� E ������ e
	{
		cout.setf(ios::uppercase);
		cout << "uppercase: " << 0.000000001 << endl;
		cout.unsetf(ios::uppercase);
	}
}

// ������
void stream_methods(){

	// ����� precision() ��������� ������ ���������� ����� ����� �����
	// ��������������� ��������
	cout.setf(ios::fixed);
	cout.precision(10);
	cout << "precision(10): " << 0.000000001 << endl;
	cout.unsetf(ios::fixed);

	
	// ����� width() ��������� ������ ������ ���� ������
	// ��������������� �� ���� �������� ������
	// ����� fill() ��������� ������ ������-�����������
	// ��������������� ��������
	cout.fill('-');
	cout.width(5);
	cout << "fill('-'): " << 123 << endl; // ??? does not work
}

// ������������