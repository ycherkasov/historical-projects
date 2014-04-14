#include <string>
#include <iostream>
#include <fstream>
#include "streams.h"

using namespace std;

void show_standard_streams_out(){

	// Стандартные потоки

	// Стандартный поток ввода (по умолчанию с клавиатуры)
	string s1;
	cin >> s1;

	// Стандартный поток ввода (по умолчанию на экран)
	cout << s1 << endl;

	// Стандартный поток ошибок, буферированный (по умолчанию на экран)
	clog << s1 << endl;

	// Стандартный поток ошибок, не буферированный (по умолчанию на экран)
	cerr << s1 << endl;

	// Вывод элементарых типов
	int i = 5;
	long l = 50;
	short s = 10;
	double d = 0.000000000000001;
	float f = 5.5678235f;
	char c = 'a';
	unsigned u = 0xffff;
	bool b = true;
	// см. enum выше
	enum{ sz = 10 };

	// интегральные типы по умолчанию выводятся как есть, в десятичной системе
	cout << "int: " << i << endl;
	cout << "long: " << l << endl;
	cout << "short: " << s << endl;
	// даже если константа в HEX
	cout << "hex uint: " << u << endl;

	// дробные числа по умолчанию обрезаются до 6 знаков
	cout << "double: " << d << endl;

	// или в экспоненциальную форму
	cout << "float: " << f << endl;

	// символы выводятся как символы, а не числа
	cout << "char: " << c << endl;

	// bool выводятся как 1 0
	cout << "bool: " << b << endl;

	// enum выводятся как int
	cout << "enum: " << sz << endl;

	// символ можно также вывести методом put
	cout << "cout.put: ";
	cout.put(c);

	// или метода write, хотя этот метод обычно используется для ввода в двоичные файлы
	cout << " cout.wtite: ";
	cout.write(&c, 1);	// не очень удобно

	// endl кроме перевода строки очищает буфер
	cout << endl;

	// указатели выводятся в HEX независимо от типа
	int* pi = &i;
	cout << "int*: " << pi << endl;

	// исключение составляют строковые литералы
	char* str1 = "atatat";
	cout << "char*: " << str1 << endl;

	// чтобы вывести их как указатель, необходимо привести к void*
	cout << "(void*)char*: " << static_cast<void*>(str1) << endl;

	// так же можно вывести как строку символьного массива
	// с помощью write
	cout.write(str1, strlen(str1));
}

void stream1(){
	// Ввод завершается нажатием Enter
	// Ввод продолжается до первого недопусимого символа
	// Например, в int разрешены только числа
	int i = 0;

	// введи 123.456
	cin >> i;

	cout << "You've entered int: " << i << endl;

	// Но введенное значение не пропадает, а остается в буфере,
	// и может попасть в следующую переменную
	double d = 0.;
	cin >> d; // сюда попадут оставшиеся символы из буфера
	cout << "You've entered double: " << d << endl;
	
	// ignore() выбрасывает оставшийся в буфере символ Enter
	cin.ignore();
}

void stream2(){
	// enter, пробелы, табуляции - по умолчанию разделители
	// введи '1 2 3'
	int a,b,c;
	cin >> a >> b >> c;
	cout << "You've entered int " << a
		<< " int " << b
		<< " int " << c
		<< endl;

	// ignore() выбрасывает оставшийся в буфере символ Enter
	cin.ignore();
}

void stream3(){
	// при вводе строк этот режим можно подавить матипулятором noskipws
	char a,b,c;
	// введи '1 2'
	cin >> noskipws >> a >> b >> c;
	cout << "You've entered char " << a
		<< " char " << b
		<< " char " << c
		<< endl;

	// ignore() выбрасывает оставшийся в буфере символ Enter
	cin.ignore();
}

void stream4(){
	// парой методам put/write являются get/read;
	char c1, c2;
	cin.get(c1);
	cin.ignore();
	cin.read(&c2, 1);

	cout << "You've entered char " << c1
		<< " char " << c2
		<< endl;

	// ignore() выбрасывает оставшийся в буфере символ Enter
	cin.ignore();
}

void input_strings1(){
	// ввести строку можно тем же методом get 
	// с указанием количества символов
	// это можно сделать только с массивом
	char s[50];
	// на последнюю позицию на всякий случай будет записан '\0'
	cin.get(s, 50);

	// но символ '\n' все равно придется удалить
	cin.ignore();
}

void input_strings2(){

	// при помощи ignore можно удалять любое количество символов
	// стандартный паттерн исползования - удалить все от enter до конца строки
	char s[50];
	cin.get(s, 50);
	cin.ignore();
}

void input_strings3(){
	// для ввода string используйте функцию getline
	string s;
	
	// '.' необязательный параметр, символ окончания ввода
	std::getline(cin, s, '.'); 
}

void stream_state1(){
	// Каждый поток может находится в 4 состояниях
	// good - все ок
	// fail - небольшая ошибка, например введен неожиданный символ. Можно очистить поток и попробовать вновь.
	// bad - фатальная ошибка, работа с потоком невозможна
	// eof - окончание ввода (для ввода это Ctrl-Z)

	// флаги состояния можно устанавливать функцийе setstate() или clear()

	// Смоделируем ситуацию fail
	int aint[10] = {};
	size_t i = 0;
	// вводим 12345e
	while((cin >> aint[i++]) && (i < 10));
	
	// на символе e ввод слоамется
	if(cin.fail()){
		// этим же методом можно устанавливать флаги
		cin.clear(/*ios_base::goodbit*/);
		cout << "Fail" << endl;
	}

	// можно сделать так, что по ситуации fail/bad/eof будет бросаться исключение
	cin.exceptions(ios::badbit|ios::failbit);

	// с параметром 0 исключения отменяются
	cin.exceptions(0);
}

void stream_state2(){

	// ввод до Ctrl-Z 
	// (обязательно рассавить скобки для соблюдения приоритета операций)
	char x;
	while(!(cin >> x)){
		if(cin.eof()){
			cout << "EOF" << endl;
		}
	}

}

void file_stream1(){
	// чтение строк из текстового файла
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