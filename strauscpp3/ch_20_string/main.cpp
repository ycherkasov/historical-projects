#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

void show_string(){
	// string - это на самом деле шаблон basic_string
	basic_string<char, char_traits<char> > str;

	// char_traits - это шаблон свойств, обеспечивающий следующую информацию:
	// - типы символов
	// - особенности их приведения к интегральным типам
	// - особенности низкоуровневых операций с массивами символов
	// - особенности ввода-вывода

	// Инициализация
	string s1;
	string s2(s1);
	string s3("str");
	// нельзя инициализировать символом
	
	s1 = 'a';
	// ..., а присвоить - можно

	// можно присваивать c-строки, при будет не будет вызван лишний конструктор
	s2 = "atatat";

	// стандартные алгоритмы менее полезны для строк,
	// чем дл других контейнеров
	// для поиска подстрок, слияния и т.п. лучше использовать
	// встроенные операции
	
	// преобразорвания в C-строку (с \0) и в мапссив (без \0)
	const char* c1 = s3.c_str();
	const char* c2 = s3.data();

	// можно сравнивать c-строки, при будет не будет вызван лишний конструктор
	bool b = (s2 == "atatat");

	// при поиске возвращается индекс начала подстроки или npos
	string::size_type s = s2.find("at");
	if(s != string::npos){
		// вывод строк
		cout << s2 << endl;
	}

	// подстроки
	string s4 = s3.substr(0,2);

	// функции для работы с ASCII
	// можно использовать в алгоритмах, включая модифицирующие
	// isalpha, isdigit, ispunct, tolower, ...
	std::transform(s3.begin(), s3.end(), s3.begin(), std::toupper);
}

int main(){

	show_string();

	return 0;
}
