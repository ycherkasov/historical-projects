#pragma once
#include <iostream>
using std::cout;
using std::endl;

// Глава 16.2 Джосаттиса
// Размещение классов в памяти
// Оптимизация пустого базового класса
// Параметры как базовые классы

// Нулевой размер пустого класса невозможен
// На этот факт завязаны
// — уникальность адресов
// — в том числе, уникальность адресов элементов массива
// — адресная арифметика

//оптимизация пустых баз позволяет нахлёстывать поля наследника на балластный хвост пустой базы.
//НО! Нахлёстывать однотипные объкты уже нельзя, т.к. это разрушает уникальность.

struct A {};
struct B : A{
	char b; // offsetof(b) = 0, sizeof(B) = 1
};
struct C : A{
	A c; // offsetof(c) = 1, sizeof(C) = 2
};

struct base1{typedef int INT;};
struct base2{};
struct derived : public base1, public base2{
	derived(){};
};

void simple_size(){
	A a = {}; B b; C c;
	cout << "sizeof A " << sizeof(a) << endl;
	cout << "sizeof B " << sizeof(b) << endl;
	cout << "sizeof C " << sizeof(c) << endl; // тоже 1, хотя должно быть 2 - оптимизация

	base1 b1 = {};
	base2 b2 = {};
	derived d;
	cout << "sizeof base1 " << sizeof(a) << endl;
	cout << "sizeof base2 " << sizeof(b) << endl;
	cout << "sizeof derived " << sizeof(c) << endl;
}

// Наследование параметров шаблона:
template <typename T1,typename T2>
class my_derived : public T1, public T2{};

// Наследование одного параметра и агрегирование другого
template <typename Base,typename Member>
class base_memeber_pair : public Base{
private:
	Member _m;
public:
	base_memeber_pair(const Base& b, const Member& m) : Base(b), _m(m){}

	// Доступ к базе и члену
	const Base& first() const {return static_cast<const Base&>(this);}
	Base& first() {return static_cast<Base&>(this);}
	const Member& second() const {return _m;}
	Member& second() {return _m;}
};
