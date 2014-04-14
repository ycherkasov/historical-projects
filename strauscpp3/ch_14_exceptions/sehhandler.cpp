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
	__try{
		cout << "In try 1" << endl;

		__try{
			cout << "In try 2.1" << endl;
			*p = 13;
			cout << "In try 2.2" << endl;
		}
		// ���� ������� � ����� ������
		__finally{
			cout << "In finally" << endl;
			BOOL abnormal = AbnormalTermination();
			cout << "Abnormal = " << abnormal << endl;
		}

	}
	// � ���� - � ����������� �� ���� �������� filter (��. MSDN __except keyword [C])
	__except( filter( GetExceptionCode(), GetExceptionInformation() ) ) {
		cout << "In except" << endl;
	}
}