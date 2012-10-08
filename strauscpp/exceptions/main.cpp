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
	
	// int and float zero div exception
	show_zero_div_exception();
    show_fpe_zero_div();
	
	show_controlled();
	//show_standard_handlers();
	test_autoptr();

	return 0;
}

