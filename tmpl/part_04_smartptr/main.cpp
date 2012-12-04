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
counting_ptr<hold_me> show_pass_ptr(counting_ptr<hold_me> ptr){
	// �� ����� counter = 2
	ptr->f();
	// ���������� ����������, counter = 3
	counting_ptr<hold_me> new_ptr = ptr;
	
	// ������� �� ��������, counter = 4
	return new_ptr;
	// ���������� �������� � ���� ���������
	// counter = 3
	// counter = 2
}

// ��� ������������ ������ ��������� �� ��������� ������
void show_refcounting_ptr(){
	// �������������, counter = 1
	counting_ptr<hold_me> ptr(new hold_me());

	// �������� �� ��������, counter = 2
	counting_ptr<hold_me> new_ptr = show_pass_ptr(ptr);
	// ����� ������ �� ������� �� �������� counter = 2
	new_ptr->f();
	// ���������� �������� � ���� ���������
	// counter = 1
	// counter = 0
}


int main(){

	//show_simple_ptr();
	show_refcounting_ptr();

	return 0;
}