#pragma once
#include <exception>

// Структура исключения
struct range_error_t : public std::exception{
	int _range;

	// Конструктор определен таким образом для отладки
	range_error_t(int r) throw() { // не бросает исключений
		_range = r;
	}

	// Конструктор копирования вызывается только если ловить по значению
	range_error_t(const range_error_t& r)  throw()  { // не бросает исключений
		_range = r._range;
	}
	// Объект исключения может копироваться сколько угодно раз, 
	// и это копирование не должно в свою очередь бросать исключений 
	// (если бросит — программа будет убита по terminate).
	// Например конструкторы std::string исключения бросают, 
	// кроме конструкторов по умолчанию string() throw();
};

/* Также shared_ptr — хорошее решение.
struct my_exception
{
	shared_ptr<string> m_pStr;
public:
	explicit my_exception(const string& str) : m_pStr(new string(str)) {}
	const string& what() const { return *m_pStr; }
};
*/

void show_exceptions();
void test_overflow(short a, short b);

