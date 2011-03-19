#include <iostream>
#include <typeinfo>

#include "virtual.h"
#include "simple.h"
#include "final.h"
#include "multimethod.h"
#include "member_pointer.h"

using std::cout;
using std::endl;

// ������������ �������� �������������� ������������
void show_simple(){

	size_t sz = 0;

	{
		control_t control;
		// ����� � ����� ������� ������ ������
		// control_t = 8
		sz = sizeof(control);
		control.draw();
		control.hide();
		control.show();
	}

	{
		button_t button;
		// � ������ ������ control_t + button_t = 12
		sz = sizeof(button);

		// ���������������� � button_t ������
		// �������� ����������
		button.push();
		button.draw();

		// �� ����������������� ������
		// ���������� �� ������������� ������
		button.hide();
		button.show();
	}

	{
		slider_t slider;
		// � ������ ������ control_t + slider_t = 20
		sz = sizeof(slider);
		
		// ���������� ���� �����
		slider.move(0);

		// ���������� ���������������� ������
		slider.draw();
		slider.hide();
		slider.show();
	}

	{
		// ����� ������ ���������� ����������� control_t()
		// ������������������ �����:
		// control_t->slider_t->control_t->button_t->window_draw
		// �.�. ������������ ���������� � �������, ���������� ��� ������������ 
		// (��. simple.h)
		window_draw drawer;

		// � ������ ������ control_t x2 + button_t + slider_t + window_draw = 36
		// �.�. ����� control_t ������� 2 ����
		sz = sizeof(drawer);

		// ���������� ���������������� ������
		drawer.draw();
		drawer.hide();
		drawer.show();

		// ���������� ������ ���������
		// (������ �� ��� ����� ������ 1 �����, ������� �������� ���������)
		drawer.move(0);
		drawer.push();
	}
}

// ������������ ������������ �������������� ������������
void show_vitrual(){

	code_formatter formatter(1);
	formatter.generate(cpp_lang);
}

// ������������ ���������� ������������
void show_final(){

	class more : public final{
		int i;
	};
	// ��������� ���������� �� �������������:
	//more m;
	//more* mp = new more();
	//delete mp;

	//error C2248: 'finalizer::finalizer' : cannot access private member declared in class 'finalizer'
	// ������ ������� ����������� ����� �� ����������, �� �����������
}

// ������������ ����������� ���� � RTTI
void show_typeid_rtti(){

	code_formatter formatter(1);
	formatter.generate(cpp_lang);

	// ��� upcast ��� crosscast dynamic_cast ������� ������������

	// ���������� �������������� (upcast)
	cpp_code_generator* cpp_gen = dynamic_cast<cpp_code_generator*>(&formatter);

	// ������������ �������������� (crosscast)
	php_code_generator* php_gen = dynamic_cast<php_code_generator*>(cpp_gen);

	// ���������� �������������� (upcast)
	code_generator_base* bs = dynamic_cast<php_code_generator*>(php_gen);

	// ���������� �������������� (downcast)
	code_formatter* cf = dynamic_cast<code_formatter*>(bs);

	// ���� �������������� ���������, �� ������������� 0-���������
	final* fin = dynamic_cast<final*>(cf);
	if( fin == 0 )
		cout << "Wrong dynamic cast" << endl;

	// ���� ������������� � ������, �� ��� ������� ��������� ���������� bad_cast
	try	{
		final& fin1 = dynamic_cast<final&>(formatter);
	}
	catch ( const std::bad_cast& e)	{
		cout << e.what() << endl;
	}

	// ���������� � ����:

	// �� ����� ����
	const type_info& ti1 = typeid(final);
	cout << ti1.name() << endl;

	// �� ����� �������
	const type_info& ti2 = typeid(formatter);
	cout << ti2.name() << endl;

	// ��� �������� ����
	const type_info& ti3 = typeid(int);
	cout << ti3.name() << endl;

	// ���������� ������������� ������� �� ����������

	// ����� before ����� ��� ����������
	int b = ti1.before(ti2);
	b = ti1.before(ti3);
}

// ������������ �������������
void show_multimethod(){

	// ���������� ����� � ����������� �� ������ 
	// � ����������� ��������
	// �������� ��� � ��������, ��� � ��� ��
	derived_a da;
	derived_b db;

	da.multiop(da);
	da.multiop(db);

	db.multiop(da);
	db.multiop(db);

	db.multiop( dynamic_cast<base&>(da) );
	db.multiop( dynamic_cast<base&>(db) );
}

// ����������� ���������� �� �����
void show_member_pointer(member_pointer* p){

	// ��������� �� ���� ����� �������� � ������� 
	// �������� & � ��������� ������������������ ����� ������
	p_mem s = &member_pointer::start;
	p_mem ss = &member_pointer::stop;

	// ������ �����
	p->start();

	// ��������� ����� ����� ���������
	member_derived* d = static_cast<member_derived*>(p);
	(d->*s)();
	(p->*ss)();

	member_derived md;
	(md.*s)();

	// ��������� �� ������� ����� ������������, ����� ��� ������� ����������
	// ����� ��������� ��������� �� ���� �������� ������ ��������� �� �����������
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
