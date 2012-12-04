#include "properties.h"
#include "func_types.h"
#include <iostream>

using std::cout;
using std::endl;

// Демонстрация простой аккумуляции и возникаюшего при этом переполнения
void show_simple_accum(){
	int iarr[] = {1,2,3,4,5};
	int i_sum = simple_accumulate<int>(iarr , iarr + sizeof(iarr)/sizeof(int) );
	cout << "simple accumulate integer: " << i_sum << endl;

	char name[] = "templates";
	char ch_sum = simple_accumulate<char>( name , name + sizeof(name) - 1 );
	cout << "simple accumulate character: " << ch_sum << endl;
}

// Демонстрация свойств типа (traits)
void show_traits(){
	int iarr[] = {1,2,3,4,5};
	int i_sum = traits_accumulate<int>(iarr , iarr + sizeof(iarr)/sizeof(int) );
	cout << "traits accumulate integer: " << i_sum << endl;

	typedef simple_accumulator_traits<char>::AccType RetType;
	char name[] = "templates";
	RetType ch_sum = traits_accumulate<char>( name , name + sizeof(name) - 1 );
	cout << "traits accumulate character: " << ch_sum << endl;
}

// Демонстрация свойств-значений (value traits)
void show_value_traits(){
	int iarr[] = {1,2,3,4,5};
	int i_sum = traits_vals_accumulate<int>(iarr , iarr + sizeof(iarr)/sizeof(int) );
}

// Демонстрация параметризованных свойств
void show_parametrics_traits(){
	int iarr[] = {1,2,3,4,5};
	int i_sum = custom_accumulator_traits<int>::accum(iarr , iarr + sizeof(iarr)/sizeof(int) );

}

// Демонстрация стратегий
void show_strategies(){
	int iarr[] = {1,2,3,4,5};
	int isum = strategy_accumulator_traits<int>::accum( iarr , iarr + sizeof(iarr)/sizeof(int) );

	// Как видно, аккумулирование умножением дает 0, т.к. аккумулирующий элемент инициализируется нулем
	// Вероятно, необходим элемент = 1, а также нулевой элемент, являющийся частью стратегии
	int imlpl = strategy_accumulator_traits<int, MultPolicy>::accum( iarr , iarr + sizeof(iarr)/sizeof(int) );
}

// Демонстрация стратегий с шаблонными аргументами 
void show_strategies_tmpl_args(){
	int iarr[] = {1,2,3,4,5};
	int isum = strategy_tmpl_accumulator_traits<int>::accum( iarr , iarr + sizeof(iarr)/sizeof(int) );
	int imlpl = 
		strategy_tmpl_accumulator_traits<int, MultPolicyT >::accum( iarr , iarr + sizeof(iarr)/sizeof(int), 1 );
}

// ------------------- Демонстрация функций типа -------------------

// Функция для тестирования типа как класса
template<typename T>
void check_is_class(){
	if(is_class<T>::Yes){
		cout << typeid(T).name() << " is a class"<< endl;
	}
	else{
		cout << typeid(T).name() << " is not a class"<< endl;
	}
}

// Демонстрация определения типа как класса
void show_is_class(){
	// НЕ РАБОТАЕТ ЗАРАЗА!!!
	//check_is_class<MyClass>();
	//check_is_class<MyStruct>();
	//check_is_class<MyUnion>();
	//check_is_class<int>();
	//check_is_class<E>();
}

int main(){

	show_simple_accum();
	show_traits();
	show_value_traits();
	show_parametrics_traits();
	show_strategies();
	show_strategies_tmpl_args();

	return 0;
}
