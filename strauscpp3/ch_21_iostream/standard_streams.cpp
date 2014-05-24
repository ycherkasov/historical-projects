#include <string>
#include <iostream>
#include <fstream>
#include "streams.h"

using namespace std;

void show_standard_streams_out(){

	// ����������� ������

	// ����������� ����� ����� (�� ��������� � ����������)
	string s1;
	cin >> s1;

	// ����������� ����� ����� (�� ��������� �� �����)
	cout << "cout: " << s1 << endl;

	// ����������� ����� ������, �������������� (�� ��������� �� �����)
	clog << "clog: " << s1 << endl;

	// ����������� ����� ������, �� �������������� (�� ��������� �� �����)
	cerr << "cerr: " << s1 << endl;

	// ��� �������� ������ ���������� ��������� �����-������� (sentry), 
	// ����������� �������������

	// ����� ����������� �����
	int i = 5;
	long l = 50;
	short s = 10;
	double d = 0.000000000000001;
	float f = 5.5678235f;
	char c = 'a';
	unsigned u = 0xffff;
	bool b = true;
	// ��. enum ����
	enum{ sz = 10 };

	// ������������ ���� �� ��������� ��������� ��� ����, � ���������� �������
	cout << "int: " << i << endl;
	cout << "long: " << l << endl;
	cout << "short: " << s << endl;
	// ���� ���� ��������� � HEX
	cout << "hex uint: " << u << endl;

	// ������� ����� �� ��������� ���������� �� 6 ������
	cout << "float: " << f << endl;
	
	// ��� � ���������������� �����
	cout << "double: " << d << endl;

	// ������� ��������� ��� �������, � �� �����
	cout << "char: " << c << endl;

	// bool ��������� ��� 1 0
	cout << "bool: " << b << endl;

	// enum ��������� ��� int
	cout << "enum: " << sz << endl;

	// ������ ����� ����� ������� ������� put
	cout << "cout.put: ";
	cout.put(c);

	// ��� ������ write, ���� ���� ����� ������ ������������ ��� ����� � �������� �����
	cout << " cout.wtite: ";
	cout.write(&c, 1);	
	// �� ����� ������, � ���� �� �� ������������� \0

	// endl ����� �������� ������ ������� �����
	cout << endl;

	// ��������� ��������� � HEX ���������� �� ����
	int* pi = &i;
	cout << "int*: " << pi << endl;

	// ���������� ���������� ��������� ��������
	char* str1 = "atatat";
	cout << "char*: " << str1 << endl;

	// ����� ������� �� ��� ���������, ���������� �������� � void*
	cout << "(void*)char*: " << static_cast<void*>(str1) << endl;

	// ��� �� ����� ������� ��� ������ ����������� �������
	// � ������� write
	cout.write(str1, strlen(str1));

	// ��������� �������� >> << �������
	// cout <<	d=f << '\n'; ->
	// (cout <<	d)=(f << '\n');
}

void stream1(){
	// ���� ����������� �������� Enter
	// ���� ������������ �� ������� ������������ �������
	// ��������, � int ��������� ������ �����
	int i = 0;

	// ����� 123.456
	cin >> i;

	cout << "You've entered int: " << i << endl;

	// �� ��������� �������� �� ���������, � �������� � ������,
	// � ����� ������� � ��������� ����������
	double d = 0.;
	cin >> d; // ���� ������� ���������� ������� �� ������
	cout << "You've entered double: " << d << endl;
	
	// ignore() ����������� ���������� � ������ ������ Enter
	cin.ignore();
	// ��� enum ����� ������ ����������� operator <<
}

void stream2(){
	// enter, �������, ��������� - �� ��������� �����������
	// ����� '1 2 3'
	int a,b,c;
	cin >> a >> b >> c;
	cout << "You've entered int " << a
		<< " int " << b
		<< " int " << c
		<< endl;

	// ignore() ����������� ���������� � ������ ������ Enter
	cin.ignore();
}

void stream3(){
	// ��� ����� ����� ���� ����� ����� �������� ������������� noskipws
	char a,b,c;
	// ����� '1 2'
	cin >> noskipws >> a >> b >> c;
	cout << "You've entered char " << a
		<< " char " << b
		<< " char " << c
		<< endl;

	// ignore() ����������� ���������� � ������ ������ Enter
	cin.ignore();
}

void stream4(){
	// ����� ������� put/write �������� get/read;
	char c1, c2;
	cin.get(c1);
	cin.ignore();
	cin.read(&c2, 1);

	cout << "You've entered char " << c1
		<< " char " << c2
		<< endl;

	// ignore() ����������� ���������� � ������ ������ Enter
	cin.ignore();
}

void input_strings1(){
	// ������ ������ � ��������� ����� ��� �� ������� get 
	// � ��������� ���������� ��������
	// ��� ����� ������� ������ � ��������
	char s[50] = {};

	// �� ��������� ������� �� ������ ������ ����� ������� '\0'
	cin.get(s, 50);

	cout << "You've entered:  " << s
		<< endl;

	cin.ignore();

	char s1[50] = {};

	// ���� �� ������� . (����� atatat.a)
	cin.get(s1, 50, '.');

	cout << "You've entered:  " << s1
		<< endl;

	// remove not only /n, but delimiter character as well
	// (2 last characters)
	cin.ignore();
	cin.ignore(1, '.');

	if (cin.fail()){
		cin.clear(/*ios_base::goodbit*/);
		cout << "Fail" << endl;
	}

}

void input_strings2(){

	// TODO: ����������� ��������
	// ��� ������ ignore ����� ������� ����� ���������� ��������
	// ����������� ������� ������������ - ������� ��� �� enter �� ����� ������
	char s[50];
	cin.get(s, 50);
	cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

	cout << "You've entered:  " << s
		<< endl;

	if (cin.fail()){
		cin.clear(/*ios_base::goodbit*/);
		cout << "Fail" << endl;
	}
}


void input_strings3(){
	// ��� ����� ����� ����������� ������� getline
	// ��� ������������� ��������� ignore()
	string s;

	// ���� ����� �������������� ��������, ������ ��������� �����
	std::getline(cin, s);

	// ����� ��������� ���������� ������� ����������� ��������
	// ��������� ��������� �������� cin.gcount()
	cout << "Entered  " << s << ", "<< cin.gcount() << " symbols" << endl;

	if (cin.fail()){
		cin.clear(/*ios_base::goodbit*/);
		cout << "Fail" << endl;
	}
}

void input_strings4(){
	// ��� ����� string ����������� ������� getline
	// �� ������ <string>
	string s;
	
	// '.' �������������� ��������, ������ ��������� �����
	std::getline(cin, s, '.');

	// std::getline() perforas only ignore(),
	// so you should manually remove delimiter
	cin.ignore(1, '.');

	cout << "Entered:  " << s << endl;

	if (cin.fail()){
		cin.clear(/*ios_base::goodbit*/);
		cout << "Fail" << endl;
	}
}

void stream_state(){
	// ������ ����� ����� ��������� � 4 ����������
	// good - ��� ��
	// fail - ��������� ������, �������� ������ ����������� ������. ����� �������� ����� � ����������� �����.
	// bad - ��������� ������, ������ � ������� ���������� (�������� � ���� ��������� fail)
	// eof - ��������� ����� (��� ����� ��� Ctrl-Z)

	// ����� ��������� ����� ������������� �������� setstate() ��� clear()

	// ����������� �������� fail
	int aint[10] = {};
	size_t i = 0;
	// ������ ������ � ����� 1235e67
	cin >> skipws;

	// !! ����� �� ������ ������������ ����� ��� ������ operator void*()
	// �.�. ��� ����������� nullptr ��������������� �� ��������� �����
	while((cin >> aint[i++]) && (i < 10));

	// ����� ����������� ������� ������ �� ���������
	for (int i = 0; i < 10; i++){
		cout << aint[i] << '\n';
	}
	
	// ...�� ������� e ���� ���������
	if(cin.fail()){
		// ���� �� ������� ����� ������������� �����
		cin.clear(/*ios_base::goodbit*/);
		cout << "Fail" << endl;
	}

	// ����� ��������� ����� ������ �������� rdstate()
	ios::iostate state = cin.rdstate();

	// ����� ������� ���, ��� �� �������� fail/bad/eof ����� ��������� ����������
	cin.exceptions(ios::badbit|ios::failbit);

	// � ���������� 0 ���������� ����������
	cin.exceptions(0);
}

void show_place_back(){
	
	// ����������� ������� ����� ������� � ����� �����
	char s[50];
	cin.get(s, 50); // enter atatat

	cout << "Entered: " << s << endl;

	// �������� � ������ ��������� ��������� ������ (t)
	cin.unget();

	cout << "Still entered: " << s << endl;
	cout << s << endl;

	char s1[50];
	// ���������, ���� �� � ������ ������ t, �� ����� �� ����
	if (cin.peek() == 't')
		cin.get(s1, 50);

	// ������ t ����� �������� �� ������
	cout << s1 << endl;

	cin.ignore();

	// �������� ������ � ����� � ���������� ��� ���
	cin.putback('c');
	if (cin.peek() == 'c')
		cin.get(s1, 50); // enter atatat
	cout << "Entered and one symbol has been placed before: " << s1 << endl;

	cin.ignore();
}

void stream_buffers(){
	// ������ �� ��������� ������������ ����-�����
	// � ���� ������� ������� ���� streambuf

	// ��� ������ ����������������� ������ ����� ��������
	// ��������������� ����� � �������� � ���
	// ��� ��������� ������ ��������� ������ ��������
	// ��������:
	// ����������� ������ ������ � ������
	// (����� ����� � stdout)
	ifstream f("file1.txt");
	cout << f.rdbuf() << endl;

	// �������� ������ ������ � ���������� �������
	ostream hexout(cout.rdbuf());
	hexout.setf(ios::hex, ios::basefield);
	hexout.setf(ios::showbase);
	hexout << 10 << endl;

	// ��������������� ����� � ����
	ofstream out_file("out_file.txt");
	// �������� ���������� ����� cout
	streambuf* buf = cout.rdbuf();
	cout.rdbuf(out_file.rdbuf());
	cout << "Enter file contents" << endl;
	// ����������� ���������� ����� cout
	cout.rdbuf(buf);

	// �������� flush() ������������� ���������� ���������� �������
	cout.flush();

	// streambuf ����� protected ��������� ��� ������������
	// � ���������� ���������������� ��������
}

void tie_streams(){
	
	// ����� �������������� �������� "���� cout-cin"
	// ���������� ������� ��
	cin.tie(&cout);

	// ��� �������� ��� ����, ����� ������ ��������������� ��������
	// ����� ������ (����� ����� �� ���������, �.�. ����� �������������)
	// tie() ������������� ������ flush() ����� ���������� �����
	string s;
	cout << "Please enter string: ";
	cin >> s;
	
	
}

void test_me(){
	show_place_back();
}

void show_standard_streams_input(){
	stream1();
	stream2();
	stream3();
	stream4();

	input_strings1();
	input_strings2();
	input_strings3();
	input_strings4();

	stream_state();

	stream_buffers();

	tie_streams();
}