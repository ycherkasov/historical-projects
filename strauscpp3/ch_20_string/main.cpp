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
	
	// TODO: �������� �������� ��������� �������� exception (?)
	
	s1 = 'a';
	// ..., � ��������� - �����

	// ����� ����������� c-������, ��� ����� �� ����� ������ ������ �����������
	s2 = "atatat";

	// ����������� ��������� ����� ������� ��� �����,
	// ��� ��� ������ �����������
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

void str_rev(char *p) {
	char *q = p;
	while (q && *q) ++q;
	for (--q; p < q; ++p, --q){
		*p = *p ^ *q;
		*q = *p ^ *q;
		*p = *p ^ *q;
	}
}

#define SWP(x,y) (x^=y, y^=x, x^=y)

void str_rev_utf8(char *p){
	char *q = p;
	strrev(p); /* call base case */

	/* Ok, now fix bass-ackwards UTF chars. */
	while (q && *q) ++q; /* find eos */
	while (p < --q)
		switch ((*q & 0xF0) >> 4) {
		case 0xF: /* U+010000-U+10FFFF: four bytes. */
			SWP(*(q - 0), *(q - 3));
			SWP(*(q - 1), *(q - 2));
			q -= 3;
			break;
		case 0xE: /* U+000800-U+00FFFF: three bytes. */
			SWP(*(q - 0), *(q - 2));
			q -= 2;
			break;
		case 0xC: /* fall-through */
		case 0xD: /* U+000080-U+0007FF: two bytes. */
			SWP(*(q - 0), *(q - 1));
			q--;
			break;
	}
}

void show_c_string_rev(){

	char str[] = "atatat";
	printf("%s ", str);
	str_rev(str);
	str_rev_utf8(str);
	printf("%s\n", str);

	// C++ way
	//std::reverse(str.begin(), str.end());
}

int main(){

	show_string();
	show_c_string_rev();
	return 0;
}
