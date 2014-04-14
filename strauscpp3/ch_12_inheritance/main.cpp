#include <iostream>
#include <typeinfo>

#include "virtual.h"
#include "simple.h"
#include "final.h"
#include "multimethod.h"
#include "member_pointer.h"

#pragma pack(1)

using std::cout;
using std::endl;

// ������������ �������� �������������� ������������
void show_simple(){

	size_t sz = 0;

	{
		control_t control;
		// ����� � ����� ������� ������ ������
		// control_t = 8 (2x int)
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
		// � ������ ������ control_t + slider_t = 20 (5x int)
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
		// ����������� control_t ����������� ������
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

	size_t sz = 0;
	// ������ �������� 3 ��������� �� ������� �����, ��� ������� ����� � ����������� �����
	code_formatter formatter(1);

	formatter.generate(cpp_lang);
	sz = sizeof(formatter);				// 36 ()
	cout << "code_formatter size = " << sz << endl;
	sz = sizeof(code_generator_base);	// 8 (int + vtbl)
	cout << "code_generator_base size = " << sz << endl;
	sz = sizeof(cpp_code_generator);	// 20 ()
	cout << "cpp_code_generator size = " << sz << endl;
	

	// ������������ �����������, �� ���� �� ������� ���������� ������������
	Final f;
	sz = sizeof(Base);		// 4 (vtbl)
	sz = sizeof(Derived1);	// 4 (vtbl)
	sz = sizeof(Derived2);	// 12 (vtbl + 2*vtbl)
	sz = sizeof(f);			// 16
}

void show_sizes(){
	
	size_t sz = 0;
	
	{
		// 4 ������ ������ ������� ����������� �������������
		class A{};
		class B1 : public A{};
		class B2 : public A{};
		class C : public B1, public B2{};

		// ������ ����� �������� 1
		sz = sizeof(A);
		sz = sizeof(B1);
		sz = sizeof(B2);
		sz = sizeof(C);
	}

		
	{
		// ������� ���� 2 ����� � ������� �����
		class A{ short a;};
		class B1 : public A{};
		class B2 : public A{};
		class C : public B1, public B2{};

		sz = sizeof(A);	// 2
		sz = sizeof(B1);// 2 (�� ��������)
		sz = sizeof(B2);// 2 (�� ��������)
		sz = sizeof(C);	// 4 (2 ����� ��������)
	}

		
	{
		// ������� ���� �� ����� ������������ �����������
		class A{ short a;};
		class B1 : virtual public A{};
		class B2 : public A{};
		class C : public B1, public B2{};

		// ������ � ������ ������ �� �������� 2 ����� ��������,
		// �� �������� ��� ��������� �� ����������� ������������
		sz = sizeof(A);	// 2
		sz = sizeof(B1);// 6 (2 �� �������� + ���������)
		sz = sizeof(B2);// 2 (�� ��������)
		sz = sizeof(C);	// 8 (2 �� �������� �� ������������� �����, 6 �� �����������)
	}

		
	{
		// ������� ��� ����� ������������ ������������
		class A{ short a;};
		class B1 : virtual public A{};
		class B2 : virtual public A{};
		class C : public B1, public B2{};

		// ������ � ������ ������ 1 ����� �������� � 2 ��������� vtbl
		sz = sizeof(A);	// 2
		sz = sizeof(B1);// 6 (2 �� �������� + ���������)
		sz = sizeof(B2);// 6 (2 �� �������� + ���������)
		sz = sizeof(C);	// 10 (2 �� �������� + 2 ���������)
	}
	
	{
		// ������� ��� ���� ����� ������������ ������������
		class A{ short a;};
		class B1 : virtual public A{};
		class B2 : virtual public A{};
		class B3 : virtual public A{};
		class C : public B1, public B2, public B3{};

		// ������ � �������������� ������ 3 ���������
		sz = sizeof(A);		// 2
		sz = sizeof(B1);	// 6 (2 �� �������� + ���������)
		sz = sizeof(B2);	// 6 (2 �� �������� + ���������)
		sz = sizeof(B3);	// 6 (2 �� �������� + ���������)
		sz = sizeof(C);		// 14 (2 �� �������� + 3 ���������)
	}

	{
		// ������� � ������� ����� ����������� ����������
		class A{ 
		public:
			short a; 
			virtual ~A(){} 
		};
		class B1 : virtual public A{};
		class B2 : virtual public A{};
		class B3 : virtual public A{};
		class C : public B1, public B2, public B3{};

		// ������ ������ �������� ������ �� 4 ����� - ��� ���� vtbl
		sz = sizeof(A);	// 6 (2 - ������ + 4 - vtbl)
		sz = sizeof(B1);// 10 (6 �� �������� + 4 - vtbl)
		sz = sizeof(B2);// 10 (6 �� �������� + 4 - vtbl)
		sz = sizeof(B3);// 10 (6 �� �������� + 4 - vtbl)
		sz = sizeof(C);	// 18 (6 �� �������� + 3*4 - vtbl)
	}
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
	// ��� ������������ �������������� �������������� ��� �� ������ ���� �����������

	// ���������� �������������� (upcast)
	code_generator_base* bs = dynamic_cast<code_generator_base*>(php_gen);

	// ���������� �������������� (downcast)
	code_formatter* cf = dynamic_cast<code_formatter*>(bs);
	// ���� ������������ �������������� ������������
	// (� ������ ���������� ����� �������� ������)
	// ������������ 0 ��� ��������� bad_cast

	// ���������� � void* � ������� dynamic_cast ����������� ��� ���������� ������
	// ������ ������� ������������ ���� (� ����� �������������� ����!)
	void* cfvoid = dynamic_cast<void*>(cf);

	// ���� �������������� ���������, �� ������������ 0-���������
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

	// ��� ������� �������� ��� �������� ������������ ��������� ��������� ���������� bad_typeid (???)
	try	{
		code_generator_base* bs = 0;
		const type_info& ti = typeid(bs);
		cout << ti.name() << endl;
	}
	catch ( const std::bad_typeid& e)	{
		cout << e.what() << endl;
	}

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

	static_member_pointer sp = &member_derived::static_init;

	// ��������� �� ������� ����� ������������, ����� ��� ������� ����������
	// ����� ��������� ��������� �� ���� �������� ������ ��������� �� �����������
	// ��������� �� ����� ����� ���� ������������ � ��������������
	// � ����������� ������ ��� ������ �������� � vtbl

	// ��������� �� ����������� ���� �������� ���������� �� ������� �������,
	// �.�. �� �������� � this
}

void show_new_delete(){
	//member_derived* m = new member_derived();
	//delete m;
}

int main(){

	show_simple();
	show_vitrual();
	show_sizes();
	show_final();
	show_typeid_rtti();
	show_multimethod();

	member_derived md;
	show_member_pointer(&md);
	show_new_delete();

	return 0;
}
