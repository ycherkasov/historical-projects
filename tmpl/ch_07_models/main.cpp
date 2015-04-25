#include "include_model.h"
#include "direct_instance.h"


// Демонстрация работоспособности модели включения
void test_include_model(){
	include_model<int> a1(1);
	include_model<double> a2(1.0);
}

// Демонстрация работоспособности модели явного инстанцирования
void show_direct_instance(){
	direct_instance<int> a1(1);
	direct_instance<double> a2(1.0);
}

// Демонстрация работоспособности модели разделения
void show_separation_model(){

}

int main(){
	test_include_model();
	show_separation_model();
	show_direct_instance();

	return 0;
}