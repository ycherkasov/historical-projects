#include "classes.h"


// --- ƒемонстраци€ методов при простом наследовании --- 

Employee::Employee(const char* n) : _name(n){

}

Employee::~Employee(){}

// ≈сли в базовом классе нет конструктора по умолчанию,
// в производном он должен быть вызван €вно

Manager::Manager(const char* n, short l , int s) 
	// ¬ производном списке инициализации нельз€ инициализировать члены базовых классов...
	: Employee(n), _level(l)/*, _salary(s)*/{	

	// ... но можно в теле конструктора!
	Employee::_salary = s;
}

Manager::~Manager(){}

void Employee::show_name(){
	cout << _name << endl;
}

void Manager::show_level(){
	cout << _level << endl;
}

void Manager::show_name(){
	cout << "Manager: ";
	// явный вызов метода базового класса
	Employee::show_name();
}


