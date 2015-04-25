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
	hold_me(const hold_me& rhs) :i(rhs.i){
		cout << "copy construction" << endl;
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
counting_ptr<hold_me> show_pass_ptr_copy(counting_ptr<hold_me> ptr){
	
	// на входе counter = 2
	// здесь должен вызваться f() для переданного объекта
	// функция неконстантная, поэтому объект отсоединяется!
	ptr->f();
	
	// создаем новый объект
	counting_ptr<hold_me> new_ptr;

	// пока просто увеличиваем счетчик
	new_ptr = ptr;

	// неконстантная операция! еще раз отсоединяем объект, создаем новый
	new_ptr->f();

	// возврат по значению
	return new_ptr;

	// разрушение объектов в зоне видимости
	// 
}

// Это демонстрация умного указателя со счетчиком ссылок
void show_refcounting_ptr(){
	// инициализация, counter = 1
	counting_ptr<hold_me> ptr(new hold_me());

	// передача по значению, counter = 2
	counting_ptr<hold_me> new_ptr = show_pass_ptr_copy(ptr);

	// после выхода из функции два объекта, у каждого counter = 2
	// (один отсоединен после вызова неконстантной операции)
	new_ptr->f();

	// разрушение объектов в зоне видимости
	// counter = 1
	// counter = 0
}


int main(){

	show_simple_ptr();
	show_refcounting_ptr();

	return 0;
}