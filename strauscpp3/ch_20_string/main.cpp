#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

void show_string(){
	// string - ��� �� ����� ���� ������ basic_string
	basic_string<char, char_traits<char> > str;

	// char_traits - ��� ������ �������, �������������� ��������� ����������:
	// - ���� ��������
	// - ����������� �� ���������� � ������������ �����
	// - ����������� �������������� �������� � ��������� ��������
	// - ����������� �����-������

	// �������������
	string s1;
	string s2(s1);
	string s3("str");
	// ������ ���������������� ��������
	
	s1 = 'a';
	// ..., � ��������� - �����

	// ����� ����������� c-������, ��� ����� �� ����� ������ ������ �����������
	s2 = "atatat";

	// ����������� ��������� ����� ������� ��� �����,
	// ��� �� ������ �����������
	// ��� ������ ��������, ������� � �.�. ����� ������������
	// ���������� ��������
	
	// ��������������� � C-������ (� \0) � � ������� (��� \0)
	const char* c1 = s3.c_str();
	const char* c2 = s3.data();

	// ����� ���������� c-������, ��� ����� �� ����� ������ ������ �����������
	bool b = (s2 == "atatat");

	// ��� ������ ������������ ������ ������ ��������� ��� npos
	string::size_type s = s2.find("at");
	if(s != string::npos){
		// ����� �����
		cout << s2 << endl;
	}

	// ���������
	string s4 = s3.substr(0,2);

	// ������� ��� ������ � ASCII
	// ����� ������������ � ����������, ������� ��������������
	// isalpha, isdigit, ispunct, tolower, ...
	std::transform(s3.begin(), s3.end(), s3.begin(), std::toupper);
}

int main(){

	show_string();

	return 0;
}
