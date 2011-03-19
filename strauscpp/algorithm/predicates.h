#pragma once
#include <iostream>
#include <functional>

using namespace std;
// -------------------  -------------------
// Определение иерархии классов для тестирования адаптеров
// и предикатов
class shape
{
	int _color;
public:
	// bad design?
	virtual void clear() = 0;

	virtual ~shape(){}
	shape(int i = 0) : _color(i){}
	int color() const {	return _color;}
	virtual void draw() = 0;
	virtual void rotate(int angle) = 0;
};

class rectangle : public shape
{
public:
	rectangle(int c) : shape(c){}
	virtual ~rectangle(){}
	virtual void clear(){delete this;}
	virtual void draw(){ std::cout << "rectangle::draw" << std::endl; }
	virtual void rotate(int angle){ std::cout << "rectangle::rotate at " << angle << std::endl; }
};

class circle : public shape
{
public:
	circle(int c) : shape(c){}
	virtual ~circle(){}
	virtual void clear(){delete this;}
	virtual void draw(){ std::cout << "circle::draw" << std::endl; }
	virtual void rotate(int angle){ std::cout << "circle::rotate at " << angle << std::endl; }
};

// -------------------  -------------------
// Предикат равенства для предыдущего класса
class shape_eq : public unary_function<shape, bool>{
	int c;
public:
	explicit shape_eq(const int i) : c(i){}
	bool operator()( const shape* s ){ return s->color() == c; }
};

// -------------------  -------------------
// Определение пользовательской версии предиката "меньше чем"
template <typename T>
class my_less_than{
public:
	my_less_than(const T& v) : _val(v){}
	bool operator()(const T& v){
		return v < _val;
	}
private:
	T _val;
};

// Функция-помощник для предыдущего класса
template <typename T>
my_less_than<T> my_less_predivate(const T& v){
	return my_less_than<T>(v);
}

// Пример функтора с сохранением состояния - аккумулятор
// Т.к. передача параметра по значению - лучше применять только для базовых типов
template <typename T>
struct sum_accum{
	sum_accum(T i = 0) : res(i){}
	void operator()(T x){ res += x; }
	T res;
};

// STL предоставляет базовые классы для функторов
// Из задача - дать стандартные имена типам аргуметов и возвращаемых значений
// unary_function & binary_function

// Функтор для демонcтрации bind1st и bind2nd
// Обязательно наследовать от binary_function или unary_function
template <typename T>
class bind_output : public binary_function<T, T, T>{
public:
	T operator()(T lhs, T rhs) const {
		cout << "left: " << lhs << ", " 
			<< "right: " << rhs << endl;
		return (lhs + rhs)/2;
	}
};
