#include "finished.h"
#include <exception>
#include <iostream>

using std::cout;
using std::endl;

// unexpected() вызывается при нарушении спецификации исключений
// В VC++ не вызывается (но можно вызвать явно catch(...) std::unexpected() )
// Стандартно выполняется unexpected() -> terminate()
// Можно подменить unexpected() через set_unexpected()
// Полным аналогом unexpected() является конструкция catch(...) в main()
void my_unexpected() {
	cout << "New unexpected" << endl;
	exit(1);
}

// terminate() вызывается при вызове неперехваченного исключения
// Стандартно вызывает abort()
// Можно подменить вызов через set_terminate(), 
// чтобы, например, вызвать exit(0) - он хотя бы вызовет деструкторы
void my_terminate() {
	cout << "New terminate" << endl;
	exit(1);
}

finished::finished(void){
}

// При выходе вернем старые обработчики - 
// это позволит создавать более гибкие системы обработки исключений
finished::~finished(void){
	set_unexpected(_old_unexpected);
	set_terminate(_old_terminate);
}

void finished::unexpected(){
	_old_unexpected = set_unexpected(my_unexpected);
}

void finished::terminate(){
	_old_terminate = set_terminate(my_terminate);
}

// Функция генерирует исключение в обход спецификации
// Должен вызваться std::unexpected() - кроме VC++
void finished::test_custom_unexpected() throw(int) {
	throw double(1);	
}

// Функция генерирует неперехваченное исключение - 
// вызовется terminate()
void finished::test_custom_terminate(){
	
	try {
		// Генерировать неперехваченное исключение
		throw 1;
	}
	catch (const double e) {
		// Ловим "не те" исключения
		cout << e << endl;
	}
}
