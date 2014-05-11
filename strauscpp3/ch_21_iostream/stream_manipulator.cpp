#include <iostream>
#include <iomanip> // manipulators with args
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
	// unsetf() - �������� �����
	// stream.copyfmt(cout) - ����������� �� ������� ������
	
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

	// ����� ��������� �����
	{
		cout.setf(ios::showbase);
		cout << "showbase: " << 10 << endl;
		cout.unsetf(ios::showbase);
	}

	// ����� ���������� �����
	{
		cout.setf(ios::showpoint);
		cout << "showpoint: " << 100.00 << endl;
		cout.unsetf(ios::showpoint);
	}

	// ������ �������� ���� �����
	{
		cout.setf(ios::showpos);
		cout << "showpos: " << 100 << endl;
		cout.unsetf(ios::showpos);
	}

	// ����� E ������ e
	{
		cout.setf(ios::uppercase);
		cout << "uppercase: " << 0.000000001 << endl;
		cout.unsetf(ios::uppercase);
	}

	// ����� ������ ���� unitbuf
	// �� ������ ��������������, �� ��������� �����������
	{
		cout.setf(ios::unitbuf);
		cout << "unitbuf: " << 10000 << endl;
		cout.unsetf(ios::unitbuf);
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
void stream_manip(){
	// ������������ ����� ��������� ����� ����� � ������, 
	// � ����� � �������������, �.�. ����� ���� ������� ����� � ������

	// ������������ - ������� ������

	// ������������ �����
	cout << "left: " << std::left << 10 << endl;

	// ������������ ������
	cout << "right: " << std::right << 20 << endl;

	// ���� ������, ����� �����
	cout << "internal: " << std::internal << -20 << endl;

	// hex + �������
	cout << "hex: " << std::hex << 0xffff << endl;

	// ����� �������� � ������
	cout << "fixed: " << std::fixed << 0.0000001 << endl;

	// ����� �������� � ���������������� ����
	cout << "scientific: " << std::scientific << 0.0000001 << endl;

	// ����� bool �������
	cout << "boolalpha: " << std::boolalpha << true << endl;
	// � ��� ����������
	cout << "noboolalpha: " << std::noboolalpha << true << endl;

	// ����� ��������� �����
	cout << "showbase: " << std::showbase << 0xaa << endl;
	// � ��� ����������
	cout << "noshowbase: " << std::noshowbase << 0xaa << endl;
	cout << std::dec;
	// ����� ���������� �����
	cout << "showpoint: " << std::showpoint << 100 << endl;
	// � ��� ����������
	cout << "noshowpoint: " << std::noshowpoint << 100 << endl;

	// ������ �������� ���� �����
	cout << "showpos: " << std::showpos << 100 << endl;
	// � ��� ����������
	cout << "noshowpos: " << std::noshowpos << 100 << endl;

	// ����� E ������ e
	cout << "uppercase: " << std::uppercase << 0.000000001 << endl;
	// � ��� ����������
	cout << "nouppercase: " << std::nouppercase << 0.000000001 << endl;

	// �� ������ ��������������, �� ��������� �����������
	cout << "unitbuf: " << std::unitbuf << 10000 << endl;
	// � ��� ����������
	cout << "nounitbuf: " << std::nounitbuf << 10000 << endl;

	// ������������ - ������� �������
	
	// setprecision
	cout << "setprecision: " << std::setprecision(16) << 0.000000001 << endl;

	// ���������� - setfill(), setflag()
	// ���������� - setiosflag(), resetiosflag()

	// ����������� ��������� �����
	cout << "setbase 16: " << std::setbase(16) << 10 << endl;
	cout << "setbase 8: " << std::setbase(8) << 10 << endl;
	// ������������� == setbase(10)
	cout << "setbase 12: " << std::setbase(12) << 10 << endl;
}

void formatted_input(){
	// ������������ ������������ ����� ��� �����
	// ��������, ����������� ���� skipws/noskipws

	// �� ���� ����� ������ basefield � boolalpha
	cout << "Enter true or false: ";
	bool b = true;
	cin >> std::boolalpha >> b;
	cin >> std::noboolalpha;
	cout << "You entered: " << b << endl;

	int i = 0;
	cout << "Enter hexidecimal number: ";
	cin >> std::setbase(16) >> i;
	cin >> std::setbase(0);
	cout << "You entered: " << i << endl;

	// setw ������ ����������� ����� �� �����
	// ����� ����������� � ���������
	char a[10] = {};
	cin >> std::setw(sizeof(a)) >> a;
	cout << "You entered: " << a << endl;
}

// ���������������� ������������
// ��� ����������
std::ostream& tab(std::ostream& os){
	return (os << '\t');
}

// � ����������� - �����-��������
template <typename T> class binary;
template <typename T>
ostream& operator<<(ostream&os, const binary<T>& t);

template <typename T>
class binary{
public:
	binary(T n) :_num(n){}
	friend ostream& operator << <> (ostream&os, const binary<T>& t);
private:
	T _num;
};

template <typename T>
inline ostream& operator << (ostream&os, const binary<T>& t){
	T MAX = numeric_limits<T>::max();
	T bit = ~(MAX >> 1);
	while (bit) {
		os << (t._num & bit ? '1' : '0');
		bit >>= 1;
	}
	return os;
}
void show_user_manip(){
	cout << "Tab user" << tab << " manip" << endl;

	cout << "User manip with params: " << binary<unsigned short>(16) << endl;
	// �����-�������� ����� ���� ����������� ��� ������� ���������� ������
	// ���������������� ����� (���, ����� � ��)
}

void show_formatted_streams(){
	stream_flags();
	stream_methods();
	stream_manip();
	formatted_input();
	show_user_manip();
}
