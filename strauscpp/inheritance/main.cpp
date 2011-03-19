#include <iostream>
#include <typeinfo>

#include "virtual.h"
#include "simple.h"
#include "final.h"
#include "multimethod.h"
#include "member_pointer.h"

using std::cout;
using std::endl;

// Демонстрация простого множественного наследования
void show_simple(){

	size_t sz = 0;

	{
		control_t control;
		// Здесь и далее считаем размер класса
		// control_t = 8
		sz = sizeof(control);
		control.draw();
		control.hide();
		control.show();
	}

	{
		button_t button;
		// в объект входит control_t + button_t = 12
		sz = sizeof(button);

		// Переопределенные в button_t методы
		// работает перекрытие
		button.push();
		button.draw();

		// Не переропределенные методы
		// вызываются из родительского класса
		button.hide();
		button.show();
	}

	{
		slider_t slider;
		// в объект входит control_t + slider_t = 20
		sz = sizeof(slider);
		
		// вызывается свой метод
		slider.move(0);

		// вызываются переопределенные методы
		slider.draw();
		slider.hide();
		slider.show();
	}

	{
		// здесь дважды вызывается конструктор control_t()
		// последовательность такая:
		// control_t->slider_t->control_t->button_t->window_draw
		// Т.е. конструкторы вызываются в порядке, прописаном при наследовании 
		// (см. simple.h)
		window_draw drawer;

		// в объект входит control_t x2 + button_t + slider_t + window_draw = 36
		// Т.е. класс control_t включен 2 раза
		sz = sizeof(drawer);

		// вызываются переопределенные методы
		drawer.draw();
		drawer.hide();
		drawer.show();

		// вызываются методы родителей
		// (каджый из них имеет только 1 копию, поэтому работает нормально)
		drawer.move(0);
		drawer.push();
	}
}

// Демонстрация виртуального множественного наследования
void show_vitrual(){

	code_formatter formatter(1);
	formatter.generate(cpp_lang);
}

// Демонстрация запрещения наследования
void show_final(){

	class more : public final{
		int i;
	};
	// Следующие инструкции не компилируются:
	//more m;
	//more* mp = new more();
	//delete mp;

	//error C2248: 'finalizer::finalizer' : cannot access private member declared in class 'finalizer'
	// Нельзя создать производный класс ни статически, ни динамически
}

// Демонстрация определения типа и RTTI
void show_typeid_rtti(){

	code_formatter formatter(1);
	formatter.generate(cpp_lang);

	// для upcast или crosscast dynamic_cast требует полиморфизма

	// повышающее преобразование (upcast)
	cpp_code_generator* cpp_gen = dynamic_cast<cpp_code_generator*>(&formatter);

	// перекрестное преобразование (crosscast)
	php_code_generator* php_gen = dynamic_cast<php_code_generator*>(cpp_gen);

	// повышающее преобразование (upcast)
	code_generator_base* bs = dynamic_cast<php_code_generator*>(php_gen);

	// понижающее преобразование (downcast)
	code_formatter* cf = dynamic_cast<code_formatter*>(bs);

	// Если преобразование запрещено, то возвращаенися 0-указатель
	final* fin = dynamic_cast<final*>(cf);
	if( fin == 0 )
		cout << "Wrong dynamic cast" << endl;

	// Если преобразуется в ссылку, то при неудаче бросается исключение bad_cast
	try	{
		final& fin1 = dynamic_cast<final&>(formatter);
	}
	catch ( const std::bad_cast& e)	{
		cout << e.what() << endl;
	}

	// Информация о типе:

	// По имени типа
	const type_info& ti1 = typeid(final);
	cout << ti1.name() << endl;

	// По имени объекта
	const type_info& ti2 = typeid(formatter);
	cout << ti2.name() << endl;

	// Для базового типа
	const type_info& ti3 = typeid(int);
	cout << ti3.name() << endl;

	// Символьное представление зависит от реализации

	// Метод before нужен для сортировки
	int b = ti1.before(ti2);
	b = ti1.before(ti3);
}

// Демонстрация мультиметодов
void show_multimethod(){

	// Вызывается метод в зависимости от класса 
	// и переданного значения
	// Работает как в иерархии, так и вне ее
	derived_a da;
	derived_b db;

	da.multiop(da);
	da.multiop(db);

	db.multiop(da);
	db.multiop(db);

	db.multiop( dynamic_cast<base&>(da) );
	db.multiop( dynamic_cast<base&>(db) );
}

// Демострация указателей на члены
void show_member_pointer(member_pointer* p){

	// Указатель на член можно получить с помощью 
	// операции & к полностью квалифицированному имени класса
	p_mem s = &member_pointer::start;
	p_mem ss = &member_pointer::stop;

	// прямой вызов
	p->start();

	// косвенный вызов через указатель
	member_derived* d = static_cast<member_derived*>(p);
	(d->*s)();
	(p->*ss)();

	member_derived md;
	(md.*s)();

	// Указатель на функцию можно использовать, когда имя функции неизвестно
	// Можно присвоить указатель на член базового класса указателю на производный
}

void show_new_delete(){
	//member_derived* m = new member_derived();
	//delete m;
}

int main(){

	show_simple();
	show_vitrual();
	show_final();
	show_typeid_rtti();
	show_multimethod();

	member_derived md;
	show_member_pointer(&md);
	show_new_delete();

	return 0;
}
