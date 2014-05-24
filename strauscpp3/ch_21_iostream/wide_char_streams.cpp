#include <iostream>
#include <locale>
#include <sstream>
#include "streams.h"

using namespace std;

void wide_symbols(){
	
	// �������, �������� � ������
	wchar_t wc = L'A';
	wchar_t str[] = L"Wide char literal";
	wstring wstr(L"Wide char string");

	wcout << str << endl;
	wcout << wstr << endl;
	
	// ����� widen()
	wcout << wcout.widen('W') << endl;
	// �������� �������� narrow()
	cout << cout.narrow(L'W') << endl;

	// ��� ������ ������� read, ������� � ��������, � �� ������
	wchar_t wc1 = '\0';
	wcin.read(&wc1, 1);
	wcout << L"You entered: " << wc1 << endl;
}

void wide_locales(){
	// ����� �������� ��������� �������, 
	// ����� ���������� ������ ��� wcin � wcout
	locale current_loc_cin = wcin.getloc();
	locale current_loc_cout = wcout.getloc();

	// � ������� Windows codepage ������ 866
	locale russian_loc("rus_rus.866");
	wcin.imbue(russian_loc);
	wcout.imbue(russian_loc);

	cout << "Locale name: " << russian_loc.name() << '\n';

	wstring cyrrilic_str;
	wcin >> cyrrilic_str;
	wcout << L"������� ������: " << cyrrilic_str << endl;

	//wcin.clear(/*ios_base::goodbit*/);
	//wcin.seekg(0);
	if (wcin.fail()){
		wcin.clear(/*ios_base::goodbit*/);
	}

	// \n should be removed after wcin usage
	wcin.ignore();

	// ��� ��������� ������ ��� �� �������� � ��������
	wstring cyrrilic_str1;
	getline(wcin, cyrrilic_str1);
	wcout << L"������� ������: " << cyrrilic_str1 << endl;

	// ������ ��������� ������ �������
	wcin.imbue(current_loc_cin);
	wcout.imbue(current_loc_cout);
}

void wide_strings(){
	// �� �������� � string_streams.cpp
	// ������ � ��������������� ������� �������
	wstring cyrillic_name;
	int age = 0;

	wstringstream ss;
	
	locale current_loc_cout = wcout.getloc();
	locale russian_loc("rus_rus.866");

	// should be set without first usage!
	ss.imbue(russian_loc);
	wcout.imbue(russian_loc);

	wcout << L"������� ��� �� ��������� � ������� (����� ������): " << endl;

	ss << L"������"
		<< L' '
		<< 32;

	// ����� ������� ���� ���������� ���������� � ������ ������
	ss.seekg(0);

	ss >> cyrillic_name >> age;
	
	wcout << L"������� ��� � �������: " << cyrillic_name << wcout.widen(' ') << age << endl;


	wstring out(ss.str());
	wcout << out << endl;

	wcout.imbue(current_loc_cout);
	
	// ���������� ��� �������� �������
}


void show_wide_char_locales(){
	//wide_symbols();
	wide_locales();
	//wide_strings();
}