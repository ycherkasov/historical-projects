#pragma once
#include <deque>

using std::deque;
// В данном заголовке продемонстрировано применение закрытого наследования
// для реализации паттерна "Адаптер"


class deque_t
{
public:
	deque_t();
	deque_t( const deque_t& c );
	deque_t& operator=( const deque_t& c );
	~deque_t();

	void push_back(int val);
	void push_front(int val);
	int pop_back();
	int pop_front();
	bool is_empty();
	void clear();

protected:
private:
	deque<int> _deque;
};

// Если мы хотим унаследовать _реализацию_ без _интерфейса_ ,
// т.е. мы не хотем, чтобы в экземпляре класса stack_t были доступны
// операции класса deque_t (т.е. в стек можно добавлять только в конец)
// мы можем использовать _закрытое_ наследование - все открытые члены класса станут private
// Если же мы хотим дальше наследовать от stack_t - лучше использовать protected
// Те данные, что в deque_t были protected и public, стали private в stack_t.

// В случае закрытого наследование также не работает приведение по ссылку-указателю к базовому классу.

class stack_t : /*public*/ protected /*private*/ deque_t{
public:
	stack_t();
	~stack_t();

	void push(int val);
	int pop();
	bool is_empty();

protected:
	void test_pr();
private:	

	// Запрещение копирования и присванивания
	stack_t(const deque_t& c);
	stack_t(const stack_t& c);
	stack_t& operator=( const stack_t& c );
	stack_t& operator=( const deque_t& c );

};

// Те даннные, что в deque_t были protected и public стали в write_only_stack_t protected.
class write_only_stack_t : public stack_t{
public:
	void push(int val);
};

// Если мы хотим открыть только часть интерфейса
// можно не переопределять методы с новыми именами
// а открыть их с помощью using
class another_stack_t : private deque_t{
public:
	// using можно определять в любой части класса, и она действует на весь класс
	using deque_t::push_back;
	using deque_t::pop_back;
};

// Также паттерн "Адаптер" реализуется композицией. Но согласно 
// принципов ООП для данного паттерна закрытое наследование подходит лучше
class composite_t{
	deque_t d;
public:
	void push(int val){d.push_back(val);}
	int pop(){return d.pop_back();}
	bool is_empty(){ return d.is_empty(); }
};