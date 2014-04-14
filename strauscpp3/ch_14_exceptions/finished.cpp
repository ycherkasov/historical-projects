#include "finished.h"
#include <exception>
#include <iostream>

using std::cout;
using std::endl;

// unexpected() ���������� ��� ��������� ������������ ����������
// � VC++ �� ���������� (�� ����� ������� ���� catch(...) std::unexpected() )
// ���������� ����������� unexpected() -> terminate()
// ����� ��������� unexpected() ����� set_unexpected()
// ������ �������� unexpected() �������� ����������� catch(...) � main()
void my_unexpected() {
	cout << "New unexpected" << endl;
	exit(1);
}

// terminate() ���������� ��� ������ ���������������� ����������
// ���������� �������� abort()
// ����� ��������� ����� ����� set_terminate(), 
// �����, ��������, ������� exit(0) - �� ���� �� ������� �����������
void my_terminate() {
	cout << "New terminate" << endl;
	exit(1);
}

finished::finished(void){
}

// ��� ������ ������ ������ ����������� - 
// ��� �������� ��������� ����� ������ ������� ��������� ����������
finished::~finished(void){
	set_unexpected(_old_unexpected);
	set_terminate(_old_terminate);
}

void finished::unexpected(){
	_old_unexpected = set_unexpected(my_unexpected);
}

void finished::terminate(){
	_old_terminate = set_terminate(my_terminate);
}

// ������� ���������� ���������� � ����� ������������
// ������ ��������� std::unexpected() - ����� VC++
void finished::test_custom_unexpected() throw(int) {
	throw double(1);	
}

// ������� ���������� ��������������� ���������� - 
// ��������� terminate()
void finished::test_custom_terminate(){
	
	try {
		// ������������ ��������������� ����������
		throw 1;
	}
	catch (const double e) {
		// ����� "�� ��" ����������
		cout << e << endl;
	}
}
