#include <cmath>
#include <string>
#include <iostream>
#include <float.h>			// ���������� �������  ��� ������ � ��������� ������
#include <vector>

#include "ex.h"
#include "controlled.h"
#include "finished.h"
#include "sehhandler.h"
#include "autoptr.h"
#include "zero_divide.h"
#include "structured_exception.h"

using std::vector;
using std::cout;
using std::endl;

// �������� ��������� ������� �����
void show_base_type_ex(int b){
	
	// ��� �������� ���������� ������� �����
	// ������� ������� ���������� �� ���������� 
	// (double -> int, unsigned -> signed, etc.)
	if(b)
		throw int(1);
	else
		throw double(0.);
	// ����� �� ������ ���������� ������� �����
	// � �� �������� ������ �� ����
	//throw new int(1);
}

void show_base_type(){
	try	{
		//show_base_type_ex(1);
		show_base_type_ex(0);
	}
	catch (const int e)	{
		cout << "integer " << e << endl;
	}
	catch (const double e) {
		cout << "double " << e << endl;
	}
	// ��� ����������� ����� ������ ��� ���������� �� ���������!
	catch (const void* p){
		delete p;
	}

}

// ����� ����� ����������� 
void show_out_of_range(){
	vector<int> vi;
	vi.push_back(1);
	vi.push_back(1);
	vi.push_back(1);

	try{
		oor_exception_ex(vi);
	}
	catch(const std::exception& e){
		cout << e.what() << endl;
	}
}

void show_simple_ex(){

	triangle* pt1 = 0;
	try{
		// ����� ��� ������
		//triangle t1(4,5,6);
		
		// ����� ������� ����������
		// ���������� �� ����� ������, ������������ ������ ������
		//triangle t2(1,2,3);

		// ����� � ������ ���������� �� ����� �������� ������ �� new
		pt1 = new triangle(4,5,6);
		//pt1 = new triangle(1,2,3);

		// ������ ����������, ������� ���� �� �������
		// �� ���� ������� ��� ��������
		pt1->throw_test();

		pt1->test_nothrow();
		pt1->test_throw_some1();
		pt1->test_throw_some2();
		
	}
	// ������� ����� ���������� ����������� �������
	catch(triangle::wrong_param& w){
		cout << w.what() << endl;
		cout << w.get_wrong_param() << endl;
	}
	catch(triangle::non_triangle_exception& w){
		cout << w.what() << endl;
		cout << w.wrong_size() << endl;
	}
	// ����� ������� (�.�., ��������� ������������, ����� � ���� �����������)
	catch(triangle::triangle_exception& w){
		cout << w.what() << endl;
	}
	
	// ��������� ������ � ����� ������
	delete pt1;
}

void complex_object_exception(){

	// create a complex class with multiple inheritance
	// and ineternal classes and throw an exception from destructor
	class internal
	{
		int i;
	public:
		internal() : i(){ cout << "internal()\n"; }
		~internal(){ cout << "~internal()\n"; }

	};

	class base
	{
	public:
		base(){ cout << "base()\n"; }
		virtual ~base(){ cout << "~base()\n"; }
	};

	class derived1 : public virtual base
	{
	public:
		derived1(){ cout << "derived1()\n"; }
		~derived1(){ cout << "~derived1()\n"; }
	};

	class derived2 : public virtual base
	{
	public:
		derived2(){ cout << "derived2()\n"; }
		~derived2(){ cout << "~derived2()\n"; }
	};

	class finalizer : public derived1, public derived2{
	public:
		internal _i;
		finalizer(){
			throw std::runtime_error("Exception from constructor");
		}
		~finalizer(){ cout << "~finalizer()\n"; }
	};

	try
	{
		finalizer f;
	}
	catch (const std::exception& e)
	{
		// finalizer has not been created
		cout << e.what();
	}
}


// ������������� ������� �� 0 
void show_zero_div_exception(){
	// ����������� SE � C++ exception (������ ���� ������������� � /EHa)
    structured_exception::enableStructuredExceptions();
    try{
		set_zero_div_exception();
        cout << "Integer division by zero" << endl;
		double ret = zero_div(0.);
        cout << "Division by zero returned " << ret << endl;
	}
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }
    catch(...){
		cout << "Unknown exception" << endl;
	}
	_clearfp();
}

// ������� �� 0 � ��������� ������
void show_fpe_zero_div(){
    // ����������� SE � C++ exception (������ ���� ������������� � /EHa)
    structured_exception::enableStructuredExceptions();
    try{
        set_zero_div_exception();
        cout << "FP division by zero" << endl;
        double ret = fpe_zero_div(0.);
        cout << "Division by zero returned " << ret << endl;
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }
    catch (...){
        cout << "Unknown exception" << endl;
    }
	_clearfp();
}

// �������������� �����������
void show_controlled() {
	try {
		controlled c;
		c.controlled_method();
	}
	catch (const std::exception& e) {
		// ���������� �� ������������ ����� ���������� ����
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Unknown exception" << endl;
	}
}

void show_standard_handlers(){
	finished f;

	f.unexpected();
	f.test_custom_unexpected();

	//f.terminate();
	//f.test_custom_terminate();
}

// ������� ������������� �������� SEH
void show_seh(){
	sehhandler seh;
	seh.test_seh();
}


int main()
{
	show_base_type();
	show_simple_ex();
	show_out_of_range();
	complex_object_exception();

	// int and float zero div exception
	show_zero_div_exception();
    show_fpe_zero_div();
	
	show_seh();
	show_controlled();
	
	// does not work with VC++
	//show_standard_handlers();

	test_autoptr();

	return 0;
}

