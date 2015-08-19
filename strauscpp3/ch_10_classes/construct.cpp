#include "construct.h"
// ƒемонстраци€ примера из —аттера1 гл.4
#include "class.h"

#include <iostream>

// ------------------------------ 
// intern - конструкторы
intern::intern() : a(){
	std::cout << "intern() default constructor" << std::endl;
}

intern::intern(int i) : a(i){
	std::cout << "intern(int) constructor" << std::endl;
}

// intern - деструктор
intern::~intern(){
	std::cout << "intern destructor" << std::endl;
}

// ------------------------------ 
// construct - конструкторы

// —начала инициализируем статические массивы
const char construct::ch_array[] = "qwertyuiop";
const int construct::int_array[] = {0,1,2,3};
int construct::_counter = 0;

construct::construct() : arr(){
	std::cout << "construct() default constructor" << std::endl;
	_counter++;	// увеличить счетчик объектов
}

// —начала вызоветс€ конструктор intern()
construct::construct(int i) : intrn(i), arr(){
	std::cout << "construct(int) constructor" << std::endl;
	_counter++;
}

construct::construct(const construct& c) : intrn(c.intrn.a), arr(){
	std::cout << "construct copy constructor" << std::endl;
	_counter++;
}

// construct - деструктор
construct::~construct(void){
	std::cout << "construct destructor" << std::endl;
	_counter--;	// уменьшить счетчик объектов
}

void construct::out_static_array(){
	// —о статическими членами может работать только статический метод
	// ≈го можно вызывать как дл€ экземпл€ра класса, так и независимо, дл€ пространства имен
	// construct::out_static_array() и c.construct::out_static_array();
	std::cout << ch_array << std::endl;
}

// все передаваемые по значению параметры объ€влены через
// forward declaration
A construct::get_a(A a){
    A a1(a);
    a1.set_a5(5);
    return a1;
}

void create_static(int i)
{
	static construct stat_c1;
	if(i){
		static construct stat_c2;
	}
}

int& return_static(){
	static int res = 0;
	return res;
}

