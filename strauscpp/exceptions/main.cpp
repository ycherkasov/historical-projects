#include <cmath>
#include <string>
#include <iostream>
#include <float.h>			// объявление функций  для данных с плавающей точкой
#include <vector>

#include "ex.h"
#include "controlled.h"
#include "finished.h"
#include "sehhandler.h"
#include "autoptr.h"
#include "zero_divide.h"
#include "structured_exception.h"

using std::vector;
using std::cout;
using std::endl;

// Бросание исключний базовых типов
void show_base_type_ex(int b){
	
	// При бросании исключений базовых типов
	// накакое неявное приведение не происходит 
	// (double -> int, unsigned -> signed, etc.)
	if(b)
		throw int(1);
	else
		throw double(0.);
	// Лучше не кидать исключения базовых типов
}

void show_base_type(){
	try	{
		//show_base_type_ex(1);
		show_base_type_ex(0);
	}
	catch (const int e)	{
		cout << "integer " << e << endl;
	}
	catch (const double e) {
		cout << "double " << e << endl;
	}

}

// Здесь ловим стандартное 
void show_out_of_range(){
	vector<int> vi;
	vi.push_back(1);
	vi.push_back(1);
	vi.push_back(1);

	try{
		oor_exception_ex(vi);
	}
	catch(const std::exception& e){
		cout << e.what() << endl;
	}
}

void show_simple_ex(){

	triangle* pt1 = 0;
	try{
		// Здесь все хорошо
		//triangle t1(4,5,6);
		
		// Здесь кинется исключение
		// Деструктор не будет вызван, динамическая память утечет
		//triangle t2(1,2,3);

		// Здесь в случае исключения не будет выделена память по new
		pt1 = new triangle(4,5,6);
		//pt1 = new triangle(1,2,3);

		// Бросим исключение, которое явно не ловится
		// Но зато ловится его родитель
		pt1->throw_test();

		pt1->test_nothrow();
		pt1->test_throw_some1();
		pt1->test_throw_some2();
		
	}
	// Сначала ловим исключения производных классов
	catch(triangle::wrong_param& w){
		cout << w.what() << endl;
		cout << w.get_wrong_param() << endl;
	}
	catch(triangle::non_triangle_exception& w){
		cout << w.what() << endl;
		cout << w.wrong_size() << endl;
	}
	// Потом базовых (т.е., благодаря полиморфизму, также и всех наследников)
	catch(triangle::triangle_exception& w){
		cout << w.what() << endl;
	}
	
	// Освободим память в любом случае
	delete pt1;
}

// Целочисленное деление на 0 
void show_zero_div_exception(){
	// Транслируем SE в C++ exception (должен быть скомпилирован с /EHa)
    structured_exception::enableStructuredExceptions();
    try{
		set_zero_div_exception();
        cout << "Integer division by zero" << endl;
		double ret = zero_div(0.);
        cout << "Division by zero returned " << ret << endl;
	}
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }
    catch(...){
		cout << "Unknown exception" << endl;
	}
	_clearfp();
}

// Деление на 0 с плавающей точкой
void show_fpe_zero_div(){
    // Транслируем SE в C++ exception (должен быть скомпилирован с /EHa)
    structured_exception::enableStructuredExceptions();
    try{
        set_zero_div_exception();
        cout << "FP division by zero" << endl;
        double ret = fpe_zero_div(0.);
        cout << "Division by zero returned " << ret << endl;
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }
    catch (...){
        cout << "Unknown exception" << endl;
    }
	_clearfp();
}

// Контролируемый конструктор
void show_controlled() {
	try {
		controlled c;
		c.controlled_method();
	}
	catch (const std::exception& e) {
		// исключение из конструктора будет проброшено сюда
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Unknown exception" << endl;
	}
}

void show_standard_handlers(){
	finished f;

	f.unexpected();
	f.test_custom_unexpected();

	//f.terminate();
	//f.test_custom_terminate();
}

// Функция демонстрирует механизм SEH
void show_seh(){
	sehhandler seh;
	seh.test_seh();
}


int main()
{
	show_base_type();
	show_simple_ex();
	show_out_of_range();
	
	// int and float zero div exception
	show_zero_div_exception();
    show_fpe_zero_div();
	
	show_controlled();
	//show_standard_handlers();
	test_autoptr();

	return 0;
}

