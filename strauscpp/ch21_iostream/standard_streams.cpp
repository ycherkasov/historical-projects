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
	cout << s1 << endl;

	// ����������� ����� ������, �������������� (�� ��������� �� �����)
	clog << s1 << endl;

	// ����������� ����� ������, �� �������������� (�� ��������� �� �����)
	cerr << s1 << endl;

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
	cout << "double: " << d << endl;

	// ��� � ���������������� �����
	cout << "float: " << f << endl;

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
	cout.write(&c, 1);	// �� ����� ������

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
	// ������ ������ ����� ��� �� ������� get 
	// � ��������� ���������� ��������
	// ��� ����� ������� ������ � ��������
	char s[50];
	// �� ��������� ������� �� ������ ������ ����� ������� '\0'
	cin.get(s, 50);

	// �� ������ '\n' ��� ����� �������� �������
	cin.ignore();
}

void input_strings2(){

	// ��� ������ ignore ����� ������� ����� ���������� ��������
	// ����������� ������� ������������ - ������� ��� �� enter �� ����� ������
	char s[50];
	cin.get(s, 50);
	cin.ignore();
}

void input_strings3(){
	// ��� ����� string ����������� ������� getline
	string s;
	
	// '.' �������������� ��������, ������ ��������� �����
	std::getline(cin, s, '.'); 
}

void stream_state1(){
	// ������ ����� ����� ��������� � 4 ����������
	// good - ��� ��
	// fail - ��������� ������, �������� ������ ����������� ������. ����� �������� ����� � ����������� �����.
	// bad - ��������� ������, ������ � ������� ����������
	// eof - ��������� ����� (��� ����� ��� Ctrl-Z)

	// ����� ��������� ����� ������������� �������� setstate() ��� clear()

	// ����������� �������� fail
	int aint[10] = {};
	size_t i = 0;
	// ������ 12345e
	while((cin >> aint[i++]) && (i < 10));
	
	// �� ������� e ���� ���������
	if(cin.fail()){
		// ���� �� ������� ����� ������������� �����
		cin.clear(/*ios_base::goodbit*/);
		cout << "Fail" << endl;
	}

	// ����� ������� ���, ��� �� �������� fail/bad/eof ����� ��������� ����������
	cin.exceptions(ios::badbit|ios::failbit);

	// � ���������� 0 ���������� ����������
	cin.exceptions(0);
}

void stream_state2(){

	// ���� �� Ctrl-Z 
	// (����������� ��������� ������ ��� ���������� ���������� ��������)
	char x;
	while(!(cin >> x)){
		if(cin.eof()){
			cout << "EOF" << endl;
		}
	}

}

void file_stream1(){
	// ������ ����� �� ���������� �����
	ifstream f("file1.txt");
	while(!f.eof()){
		string s;
		getline(f, s);
		cout << s << endl;
	}
}


void show_standard_streams_input(){
	//stream1();
	//stream2();
	//stream3();
	//stream4();
}