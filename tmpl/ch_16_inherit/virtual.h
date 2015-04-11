#pragma once
#include <iostream>
using std::cout;
using std::endl;

//  Глава 16.4 Джосаттиса
// Параметризованная виртуальность

// Параметризация виртуальности методов
class non_virtual{};

class my_virtual{
public:
	virtual void foo(){}
};

template <typename VBase>
class Base : private VBase{
public:
	// Виртуальность foo() зависит от того, какой класс передан параметром
	// как базовый - с виртуальностью или без
	// Наследовать можно приватно - реализация не важна
	void foo(){ cout << "Base::foo"  << endl; }
};

template <typename IsVirtual>
class Derived : public Base<IsVirtual>{
public:
	// Виртуальность foo() зависит от того, какой класс передан параметром
	// как базовый - с виртуальностью или без
	void foo(){ cout << "Derived::foo"  << endl; }
};
