#include "by_request.h"


void show_forward_decl(){

	// ���������, � �������� ��������� ��������� �� forward declaration
	TemplatePreuse p;
}

void show_by_request() {
	// � ���� ������� ����� �����������, �� �� ������������ ������
	// � �� �������������, �.�. ��� �� ���������� => �� ��������������
	InstantByRequest<int, 0> thats_ok;
}

int main(){
	
	show_forward_decl();
	show_by_request();

	return 0;
}