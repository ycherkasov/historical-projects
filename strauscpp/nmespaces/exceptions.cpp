#include "exceptions.h"

#include <limits>
#include <iostream>
using std::numeric_limits;



char int_to_char(int i){

	// numeric_limits - ��. ������ 22.2
	if( ( numeric_limits<char>::max() < i ) || ( numeric_limits<char>::min() > i ) )
		throw range_error_t(i);
	return i;
}

void show_exceptions(){

	try{
		char a = int_to_char(35);
		a = int_to_char(5689);
	}
	// ����� �� ������
	catch (const range_error_t& e){	
		// ���������� ��������� ������������ � ������� �����
		// � ���� catch
		// ��������������� ���������� ��� ����������,
		// ��������� �� ����� catch, ���������� ������
		std::cout << "Unable convert to char " << e._range << std::endl;
	}

	try{
		char a = int_to_char(45);
		a = int_to_char(123564);
	}
	// ����� �� �������� - ��������� ����������� �����������
	// ��� ����� �� ������
	catch ( const range_error_t e ){
		std::cout << "Unable convert to char " << e._range << std::endl;
	}
	//catch(...) �����������, �.�.:
	//1. �� ��� ���������� �������� �� � ��������� ������� ���������� ��������� �� ������.
	//2. � ����������� �� ����� ���������� �� ����� ������ SEH ����������, ������� ������ �� �������.
	//3. �� ����� ������ ����������, ������� �� ������ �������� ���������������� ����� 
	// (boost::tread_interrupted, ������������� �� ��������� std::exception).
	//4. �� ����� ������� ����������, ������� ������� �� ��������� � ������� ������������, 
	// �� ��� ���������� �������� �� ������ ��� ����������.


}