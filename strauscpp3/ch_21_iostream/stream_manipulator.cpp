#include <iostream>
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

	// вывод E вместо e
	{
		cout.setf(ios::uppercase);
		cout << "uppercase: " << 0.000000001 << endl;
		cout.unsetf(ios::uppercase);
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