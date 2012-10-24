#include <iostream>
#include "simple_ptr.h"
#include "counting_ptr.h"

using std::cout;
using std::endl;

// Это класс для демонстрации возможностей умных указателей
class hold_me{
	int i;
public:
	hold_me():i(){
		cout << "construction" << endl;
	}
	~hold_me(){
		cout << "destruction" << endl;
	}
	void f(){i++;}
};

// Это демонстрация простого умного указателя без возожности присваивания
void show_simple_ptr(){
	simple_ptr<hold_me> ptr(new hold_me());
	ptr->f();
	(*ptr).f();
}

// Это функция для передачи указателя со счетчиком ссылок по значения
// При копировании сам объект остается нетронутым, умеличивается и уменьшается только счетчик
counting_ptr<hold_me> show_pass_ptr(counting_ptr<hold_me> ptr){
	// на входе counter = 2
	ptr->f();
	// копирующее присвоение, counter = 3
	counting_ptr<hold_me> new_ptr = ptr;
	
	// возврат по значению, counter = 4
	return new_ptr;
	// разрушение объектов в зоне видимости
	// counter = 3
	// counter = 2
}

// Это демонстрация умного указателя со счетчиком ссылок
void show_refcounting_ptr(){
	// инициализация, counter = 1
	counting_ptr<hold_me> ptr(new hold_me());

	// передача по значению, counter = 2
	counting_ptr<hold_me> new_ptr = show_pass_ptr(ptr);
	// после выхода из функции по прежнему counter = 2
	new_ptr->f();
	// разрушение объектов в зоне видимости
	// counter = 1
	// counter = 0
}


int main(){

	//show_simple_ptr();
	show_refcounting_ptr();

	return 0;
}