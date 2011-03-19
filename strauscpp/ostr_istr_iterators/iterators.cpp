#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <complex>
#include <valarray>

using namespace std;

// --------------- ��������� --------------- 
// ������� ������������� ��������,
// ����������� �� ����� ������
void show_out_iter()
{
	// (������, ����������� ���������, ��������� �� �����)
	// ����� � ������ ������ �������������� ��� ���������
	ostream_iterator<string> oi(cout);

	*oi = "aaa";
	++oi;
	*oi = " bbb\n";
}

// ������� ������������� ��������, ����������� �� ����� �����
// (������, ����������� ���������, ���������� � ����� ����� � ����������)
void show_istr_iter()
{
	// �������������� �������� ������� �����
	istream_iterator<string> iinput_iter(cin);

	// ����������� ������ ������
	string s1 = *iinput_iter;

	// ����������� ������ ������
	++iinput_iter;
	string s2 = *iinput_iter;

	// ������ ������� ����������
	cout << "First string: " << s1 << '\n' << "Second string: " << s2 << endl;
}

// ������� ������ ���� (������ test.txt), ��������� ����������� �����
// ������� ��������� � ���������� ��������� � ������ ���� (������ test1.txt)
// ������������� ��� ���� ���������� (����� � ������) � �������� ���������
int show_iterators()
{
	// ��������� ����� ������
	ostream_iterator<string> out_iter(cout);

	// ������ ������ �������
	*out_iter = "Input file:";

	// �������������� �������� ������� �����
	istream_iterator<string> iinput_iter(cin);
	string from = *iinput_iter;

	// ������ ������ �������
	++out_iter;
	*out_iter = "Output file:";

	++iinput_iter;
	string to = *iinput_iter;

	// ����� ����� ������� � ������ ���������
	ifstream from_file( from.c_str() );

	// ��������, ��������� � ������ �������
	// (����� ��� ���������)
	istream_iterator<string> from_it_begin(from_file);

	// ����������� ��������� ����� �� ��������� ������� ��������� �� ����� �����
	istream_iterator<string> from_it_end;

	// �������������� ������ ��������� � �������� ����������
	vector<string> b(from_it_begin, from_it_end);

	// ����������� ������
	sort(b.begin(), b.end());

	// ����� ������ ������� � ������ ���������
	ofstream to_file( to.c_str() );

	// ��������, ��������� � ������ �������
	ostream_iterator<string> to_it_begin(to_file, "\n");

	// ��������� ������ ���������� ��������
	unique_copy( b.begin(), b.end(), to_it_begin );

	// ���������� ��������� ������
	return !from_file.eof() && !to_file;
}

// --------------- for_each --------------- 
// ������� ��� ��������� 'foreach'
// ��������� ������� ���������������� ����������
void print_v(const string& s)
{
	cout << s << endl;
}

void show_foreach()
{
	vector<string> st;
	st.push_back("atatat");
	st.push_back("komatoza");
	for_each(st.begin(), st.end(),print_v);
}

// --------------- find_if --------------- 
// ����� � ����������
bool is_atatat(const string& s)
{
	return (s == "atatat");
}

void show_findif()
{
	vector<string> st;
	st.push_back("atatat");
	st.push_back("komatoza");
	vector<string>::const_iterator i = 
		find_if(st.begin(), st.end(), is_atatat);
}

// --------------- mem_fun --------------- 
// ������� for_each - 
// ��������� �������� � ������������
// ������������ ����������� �������


// --------------- math --------------- 
// ������� ������������� �������������� ����������� STL
void show_math() {

	// ����������� �����
	complex<double> c1(1. , 2. );
	complex<double> c2(1.5, 0.2);
	complex<double> c3 = c1 + c2;

	// �������������� ������
	valarray<double> vect1(10,2);
	valarray<double> vect2(5,2);
	valarray<double> vect3 = vect1 + vect2;

	// �������� �������������� �������
	// ���������� ������������ ����� � �������
	c3 = pow(c3, 2);

	// ������������ ��������
	vect3 = vect1*vect2;

	// ����������� �������� ������������ �����
	c2 = log(c1);
}


// ----------------------------------
// ����������� ��������� (������� main ����� �� ���������� ��������)
/*
int main(){}
*/


int main()
{
	// �������� � ������� main 
	// (�������� ���������� ��������� � ��������� � ������������)
	// ������ ����������� � try/catch
	try
	{
		// 1. ������������ ���������� �����/������
		show_out_iter();
		show_istr_iter();
		show_iterators();

		// 2. ������������ foreach
		show_foreach();

		// 3. ������������ find_if � ����������
		show_findif();

		// 4. ������������ mem_fun � ����������
		// �������� ������� draw ��� ������� �������� ������������ ������ 'sh'
		// list<Shape*> sh(...);
		// for_each( sh.begin(), sh.end(), mem_fun(&Shape::draw) );

		// 5. ������������ �������������� ������������
		show_math();
	}
	catch (const std::exception& e)
	{
		cout << "exception: " << e.what();
		return 1;
	}

	// ��� �������� ����� ���������� �������
	return 0;
}
