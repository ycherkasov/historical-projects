// ���� ����� �������� forward declarations ��� ���� ��������
// ��� ������������� ��������� ����� ����������
#include <iosfwd>
#include "streams.h"

using namespace std;

void show_standard_streams(){

	// ����������� ������
	show_standard_streams_out();
	show_standard_streams_input();

}


void show_file_streams(){
	show_fstream();
}



int main(){
	//test_me();

	// uncomment one by one
	show_standard_streams();
	//show_file_streams();
	//show_formatted_streams();
	//show_string_streams();
	//show_wide_char_locales();
	return 0;
}