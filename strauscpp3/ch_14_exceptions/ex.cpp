#include <string>
#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

#include "ex.h"

// Иерархия исключений



int oor_exception(std::vector<int>& v){
	return v.at(1000);
}

void oor_exception_ex(std::vector<int>& v){
	oor_exception(v);
}

triangle::triangle(double a, double b, double c) : _a(a), _b(b), _c(c), _S(), _P(){
	
	// Выделим динамическую память в тестовых целях
	_test = new int();
	// При броске исключения из конструктора она утечет
	
	// Для предотвращения следует пользоваться методикой
	// "выделение ресурса есть инициализация" (auto_ptr, wrapper, etc.)
	// и обработкой исключений в конструкторе (см. todo: ____________________)

	// Одна из сторон <= 0, кинем исключние
	if (a <= 0) throw wrong_param(a);
	if (b <= 0) throw wrong_param(b);
	if (c <= 0) throw wrong_param(c);

	// Не выполняется неравенство треугольника!
	if( (a+b <= c) || (a+c <= b) || (c+b <= a) )
		throw non_triangle_exception(a, b, c);

}

triangle::~triangle(){
	// Освободим динамическую память
	delete _test;
	cout << "triangle destruction" << endl;
}

double triangle::perimeter(){
	_P = _a + _b + _c;
	return _P;
}

void triangle::throw_test() const{
	throw test_triangle_ex(1);
}

void triangle::test_nothrow() const throw() {
	// warning!
	throw test_triangle_ex(0);
}

void triangle::test_throw_some1() const {
	throw wrong_param(0.);
}

void triangle::test_throw_some2() const {
	throw wrong_param(0.);
}

double triangle::square(){
	if(!_P)
		_P = perimeter();
	
	if(!_S)
		_S = sqrt( _P * (_P - _a) * (_P - _b) * (_P - _c) );

	return _S;
}

