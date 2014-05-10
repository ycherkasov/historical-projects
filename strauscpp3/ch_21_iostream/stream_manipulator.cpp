#include <iostream>
#include <iomanip> // manipulators with args
#include "streams.h"

using namespace std;

// Форматировать поток можно тремя способами
// - флаги
// - методы
// - манипуляторы

// Флаги
void stream_flags(){

	// Получение текущих флагов потока
	ios::fmtflags flags = cout.flags();

	// Флаги задаются для потока следующими методами
	// flags()  - аналог =
	// setf() - аналог OR
	// unsetf() - сбросить флаги
	// stream.copyfmt(cout) - скопировать из другого потока
	
	// выравнивание влево
	{
		// ios::adjustfield - для работы с группой флагов выравнивания
		cout.setf(ios::left, ios::adjustfield);
		cout << "left: " << 10 << endl;
		cout.unsetf(ios::left);
	}

	// выравнивание вправо
	{
		// ios::adjustfield - для работы с группой флагов выравнивания
		cout.setf(ios::right, ios::adjustfield);
		cout << "right: " << 20 << endl;
		cout.unsetf(ios::right);
	}

	// знак вправо, число влево
	{
		// ios::adjustfield - для работы с группой флагов выравнивания
		cout.setf(ios::internal, ios::adjustfield);
		cout << "internal: " << -20 << endl;
		cout.unsetf(ios::internal);
	}

	// hex + префикс
	{
		// ios::basefield - для работы с группой флагов системы счисления
		cout.setf(ios::hex|ios::showbase, ios::basefield);
		cout << "hex|showbase: " << 0xffff << endl;
		cout.unsetf(ios::hex|ios::showbase);
	}

	// вывод дробного с точкой
	{
		cout.setf(ios::fixed);
		cout << "fixed: " << 0.0000001 << endl;
		cout.unsetf(ios::fixed);
	}

	// вывод дробного в экспоненциальном виде
	{
		cout.setf(ios::scientific);
		cout << "scientific: " << 0.0000001 << endl;
		cout.unsetf(ios::scientific);
	}

	// вывод bool словами
	{
		cout.setf(ios::boolalpha);
		cout << "boolalpha: " << true << endl;
		cout.unsetf(ios::boolalpha);
	}

	// вывод основание числа
	{
		cout.setf(ios::showbase);
		cout << "showbase: " << 0xaa << endl;
		cout.unsetf(ios::boolalpha);
	}

	// вывод незначащих нулей
	{
		cout.setf(ios::showpoint);
		cout << "showpoint: " << 100.00 << endl;
		cout.unsetf(ios::showpoint);
	}

	// всегда выводить знак числа
	{
		cout.setf(ios::showpos);
		cout << "showpos: " << 100 << endl;
		cout.unsetf(ios::showpos);
	}

	// вывод E вместо e
	{
		cout.setf(ios::uppercase);
		cout << "uppercase: " << 0.000000001 << endl;
		cout.unsetf(ios::uppercase);
	}

	// также важный флаг unitbuf
	// не меняет форматирование, но отключает буферизацию
	{
		cout.setf(ios::unitbuf);
		cout << "unitbuf: " << 10000 << endl;
		cout.unsetf(ios::unitbuf);
	}
}

// Методы
void stream_methods(){

	// Метод precision() позволяет задать количество чисел после точки
	// устанавливается насовсем
	cout.setf(ios::fixed);
	cout.precision(10);
	cout << "precision(10): " << 0.000000001 << endl;
	cout.unsetf(ios::fixed);

	
	// Метод width() позволяет задать ширину поля вывода
	// устанавливается на одну операцию вывода
	// Метод fill() позволяет задать символ-заполнитель
	// устанавливается насовсем
	cout.fill('-');
	cout.width(5);
	cout << "fill('-'): " << 123 << endl; // ??? does not work
}

// Манипуляторы
void stream_manip(){
	// Манитуляторы могут подменять собой флаги и методы, 
	// и проще в использовании, т.к. могут быть вызваны прямо в потоке

	// Манитуляторы - аналоги флагов

	// выравнивание влево
	cout << "left: " << std::left << 10 << endl;

	// выравнивание вправо
	cout << "right: " << std::right << 20 << endl;

	// знак вправо, число влево
	cout << "internal: " << std::internal << -20 << endl;

	// hex + префикс
	cout << "hex: " << std::hex << 0xffff << endl;

	// вывод дробного с точкой
	cout << "fixed: " << std::fixed << 0.0000001 << endl;

	// вывод дробного в экспоненциальном виде
	cout << "scientific: " << std::scientific << 0.0000001 << endl;

	// вывод bool словами
	cout << "boolalpha: " << std::boolalpha << true << endl;
	// и его выключение
	cout << "noboolalpha: " << std::noboolalpha << true << endl;

	// вывод основание числа
	cout << "showbase: " << std::showbase << 0xaa << endl;
	// и его выключение
	cout << "noshowbase: " << std::noshowbase << 0xaa << endl;

	// вывод незначащих нулей
	cout << "showpoint: " << std::showpoint << 100 << endl;
	// и его выключение
	cout << "noshowpoint: " << std::noshowpoint << 100 << endl;

	// всегда выводить знак числа
	cout << "showpos: " << std::showpos << 100 << endl;
	// и его выключение
	cout << "noshowpos: " << std::noshowpos << 100 << endl;

	// вывод E вместо e
	cout << "uppercase: " << std::uppercase << 0.000000001 << endl;
	// и его выключение
	cout << "nouppercase: " << std::nouppercase << 0.000000001 << endl;

	// не меняет форматирование, но отключает буферизацию
	cout << "unitbuf: " << std::unitbuf << 10000 << endl;
	// и его выключение
	cout << "nounitbuf: " << std::nounitbuf << 10000 << endl;

	// Манитуляторы - аналоги методов
	
	// setprecision
	cout << "setprecision: " << std::setprecision(16) << 0.000000001 << endl;

	// аналогично - setfill(), setflag()
	// аналогично - setiosflag(), resetiosflag()

	// выставление основания числа
	cout << "setbase 16: " << std::setbase(16) << 10 << endl;
	cout << "setbase 8: " << std::setbase(8) << 10 << endl;
	// и даже нестандартные(?)
	cout << "setbase 12: " << std::setbase(12) << 10 << setbase(0) << endl; // setbase(0) == setbase(1s0)
}

void formatted_input(){
}

void show_formatted_streams(){
	stream_flags();
	stream_methods();
	stream_manip();
	formatted_input();
}
