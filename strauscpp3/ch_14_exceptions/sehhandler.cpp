#include "sehhandler.h"
#include "windows.h"
#include <iostream>

using std::cout;
using std::endl;


int sehhandler::filter( unsigned code, struct _EXCEPTION_POINTERS* ep ){
	
	// ������������� ��� ���������� (Access violation)
	if( code == EXCEPTION_ACCESS_VIOLATION ){
		cout << "Access violation" << endl;
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else{
		cout << "Unexpected exception" << endl;
		return EXCEPTION_CONTINUE_SEARCH;
	}

}

void sehhandler::test_seh(){
	int* p = 0x0;
	// ���������� ����
	__try{
		cout << "In try 1" << endl;

		// ��� ����������� (__except � __finally) �� �������� ����������� WinAPI ������ ������
		// ��� ������������� ��������� ������ try-except-finally ����������� ���������� ���� � �����
		__try{
			cout << "In try 2.1" << endl;
			*p = 13;
			cout << "In try 2.2" << endl;
		}
		// ���� ����������: ���� ������� � ����� ������
		__finally{
			cout << "In finally" << endl;
			
			// If the __try block terminated abnormally, the return value is nonzero.
			// If the __try block terminated normally, the return value is zero.
			BOOL abnormal = AbnormalTermination();
			cout << "Abnormal = " << abnormal << endl;
		}

	}
	// � ���� - � ����������� �� ���� �������� filter (��. MSDN __except keyword [C])
	// filter ��������� ������ � ������ ���������� SEH!
	__except( filter( GetExceptionCode(), GetExceptionInformation() ) ) {
		// EXCEPTION_CONTINUE_EXECUTION ������� ��� ��� ��������� ����������, ��������� ����������
		// ����������� � ������, ����� ������ ��� ��������� ��������� ��������
		// EXCEPTION_CONTINUE_SEARCH ���������� ���������� ���� �� �����
		// EXCEPTION_EXECUTE_HANDLER ��������� ���� ����������� __except (����� __finally)
		// ���� �� ����� ������������ ���������� ������ (�������� � ����������� �������� ������)
		// ������ ������� ����� ���������� ���������
		cout << "In except" << endl;
	}
}