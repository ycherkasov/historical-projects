#include <iostream>
#include "simple_ptr.h"
#include "counting_ptr.h"

using std::cout;
using std::endl;

// ��� ����� ��� ������������ ������������ ����� ����������
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

// ��� ������������ �������� ������ ��������� ��� ���������� ������������
void show_simple_ptr(){
	simple_ptr<hold_me> ptr(new hold_me());
	ptr->f();
	(*ptr).f();
}

// ��� ������� ��� �������� ��������� �� ��������� ������ �� ��������
// ��� ����������� ��� ������ �������� ����������, ������������� � ����������� ������ �������
counting_ptr<hold_me> show_pass_ptr_copy(counting_ptr<hold_me> ptr){
	
	// �� ����� counter = 2
	// ����� ������ ��������� f() ��� ����������� �������
	// ������� �������������, ������� ������ �������������!
	ptr->f();
	
	// ������� ����� ������
	counting_ptr<hold_me> new_ptr;

	// ���� ������ ����������� �������
	new_ptr = ptr;

	// ������������� ��������! ��� ��� ����������� ������, ������� �����
	new_ptr->f();

	// ������� �� ��������
	return new_ptr;

	// ���������� �������� � ���� ���������
	// 
}

// ��� ������������ ������ ��������� �� ��������� ������
void show_refcounting_ptr(){
	// �������������, counter = 1
	counting_ptr<hold_me> ptr(new hold_me());

	// �������� �� ��������, counter = 2
	counting_ptr<hold_me> new_ptr = show_pass_ptr_copy(ptr);

	// ����� ������ �� ������� ��� �������, � ������� counter = 2
	// (���� ���������� ����� ������ ������������� ��������)
	new_ptr->f();

	// ���������� �������� � ���� ���������
	// counter = 1
	// counter = 0
}


int main(){

	show_simple_ptr();
	show_refcounting_ptr();

	return 0;
}