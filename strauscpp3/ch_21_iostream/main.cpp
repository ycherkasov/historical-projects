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
	// uncomment by one
	//show_standard_streams();
	//show_file_streams();
	show_formatted_streams();
	return 0;
}