#include <deque>
#include <cstring>
#include <complex>

#include "func_tmpl.h"
#include "class_tmpl.h"
#include "non_type_params.h"
#include "basics_tmpl.h"

using std::string;

// Демонстрация простого шаблона функций
void show_func_template(){
	
	int i = 10;

	// Используем форму глобального namespace - ::max
	// чтобы не перепутать с std::max
	::max(7, i);
	::max(3.4, 7.6);

	std::string s1("atatat");
	std::string s2("bdbdbdb");
	::max(s1, s2);

	// Во всех трех приведенных формах определение типа произошло автоматически
	// Срабатывает т.н. "вывод аргументов шалона" - связь параметров шаблона и его аргументов
	// Можно это делать явно
	int a = ::max<int>(i, 5);

	// Для корректного инстанцирования для типа T должна быть определена операция сравнения
	// например для std::complex работать не будет
	std::complex<double> c1(0.1,0.2);
	std::complex<double> c2(.0,0.5);
	// ::max(c1, c2);
	// error C2784: 'bool std::operator >(const std::basic_string<_Elem,_Traits,_Alloc> &,const _Elem *)' 
	// : could not deduce template argument for 'const std::basic_string<_Elem,_Traits,_Alloc> &' 
	// from 'const std::complex<double>'

	// Т.е. необходимо, чтобы каждая операция для элемента шаблона была допустима

	// Таким образом код шаблона компилируется 2 раза - 
	// при проверке синтаксической правильности и при инстанцировании

	// Чтобы передавать в шаблон параметры разных типов, надо:
	// - перегрузить обычную функцию, чтобы совершалась неявная подстановка
	// - использовать несколько параметров шаблона typename
	
	// работает обычная функция с неявным привидением
	int b = ::max(1, 3.5);
	// warning C4244: 'argument' : conversion from 'double' to 'const int', possible loss of data

	// работает шаблон (форма <> - явный вызов шаблона, а не функции)
	b = ::max<>(1, 2);
}

// Демонстрация простого шаблона класса
void show_class_tmpl(){
	stack<int> s;

	s.push(1);
	s.push(2);

	// Инстанцирование происходит только для вызываемых функций-членов
	int i = s.top();
	s.pop();
	// т.е. bool empty() не инстанцируется

	stack<string> s2;
	s2.push("atatat1");
	s2.push("atatat2");
}

// Демонстрация частичной специализации
void show_parital_spec(){
	double_stack<long, int> ds0;	// специализация "второй параметр - int"
	double_stack<int, double> ds1;	// нет специализации
	double_stack<double, double> ds2;	// специализация "параметры одинаковые"
	double_stack<double*, int*> ds3;	// специализация "параметры - указатели"
}

// Демонстрация параметров по умолчанию
void show_default_param(){
	custom_stack<int> st1;
	custom_stack<int, std::deque<int> > st2;

	st1.push(1);
	st2.push(1);
}

// Демонстрация параметров, не являющихся типами 
// (шаблон класса-массива фиксированного размера)
void show_non_type_params(){

	// Это разные типы!
	fixed_array<int, 20> ar1;
	fixed_array<int, 40> ar2;

	try {
		ar1[10] = 10;
		ar2[20] = 10;
		// Здесь сработает проверка границ, бросит исключение
		ar2[200] = 10;
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;		
	}
}

// Демонстрация применние ключевых слов typename и template
void show_typename_tmpl(){
	std::list<int> v;
	v.push_back(1);
	v.push_back(2);

	print_container< std::list<int> >(v);

	test_bitset();
}

// Демонстрирует шаблонный метод шаблонного класса
void show_template_member(){
	copyable_stack<int> is;
	copyable_stack<double> ds;

	is.push(1);
	ds.push(1.0);
	ds.push(2.0);
	is = ds;
}

// Демонстрация шаблонов с шаблонным параметром
void show_tmpl_param(){
	
	// Экземпляр с параметром по умолчанию
	tmpl_stack<int> s1;
	s1.push(1);
	s1.push(2);

	// Используем другой тип и контейнер
	tmpl_stack<double, std::vector> s2;
	s2.push(1.0);
	s2.push(3.0);

	// Инстанцируем operator= (самый сложный член класса)
	s1 = s2;

	// Для шаблонов функций шаблонные параметры не допускаются!
}


// Демонстрация инициализации нулем
void show_zero_init(){
	
	// Для всех типов будет произведена инициализация нулем
	init_zero<int> iz1;
	init_zero< std::vector<int> > iz2;
	init_zero< std::string > iz3;
}

// Демонстрация строковых литералов и приведения массивов к указателям
// Преобразование массива в указатель присходит только в случае, если имя массва передается по значению
void show_literal(){

	// Ошибка: нельзя привести массив к указателю, т.к. передается по ссылке
	// error C2440: 'return' : cannot convert from 'const char *' to 'const char (&)[5]'
	// str_max_ref("aaaa", "bbbb");
	show_ref("aaaa");

	// А здесь можно привести массив к указателю, т.к. передается по значению
	str_max_val("aaaa", "bbbb");
	show_val("aaaa");

	//Для предотвращения подобных ситуаций:
	// - использовать передачу по ссылке (или перегрузку для массивов)
	// - не использовать С-строки (предпочтительно)
	// Первый способ применен в std::make_pair(f, s) - оба параметра по значению для передачи литералов
}

int main(){
	
	show_func_template();
	show_class_tmpl();
	show_parital_spec();
	show_default_param();
	show_non_type_params();
	show_typename_tmpl();
	show_tmpl_param();
	show_literal();
	
	
	return 0;
}