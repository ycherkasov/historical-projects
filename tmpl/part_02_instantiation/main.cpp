#include "by_request.h"


void show_forward_decl(){

	// Структура, в котророй определен указатель на forward declaration
	TemplatePreuse p;
}

void show_by_request() {
	// В этом шаблоне масса объявленных, но не определенных членов
	// И он компилируется, т.к. они не вызываются => не инстанцируются
	InstantByRequest<int, 0> thats_ok;
}

int main(){
	
	show_forward_decl();
	show_by_request();

	return 0;
}