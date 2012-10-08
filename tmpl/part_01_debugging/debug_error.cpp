#include <list>
#include <string>
#include <algorithm>

using namespace std;

// ------------------- Громоздкие отладочные сообщения -------------------
void show_long_error(){
	list<string> coll;

	// Здесь допущена ошибка
	// в предикат greater передан параметр шаблона int
	// когда сравнение происходит по std::string
	list<string>::iterator pos =
		find_if( coll.begin(), coll.end(), 
		bind2nd( greater</*int - ошибка!*/string>(), "A" ) );
	/*
	error C2440: 'initializing' : cannot convert from 'const char [2]' to 'int'
	There is no context in which this conversion is possible
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(15) : 
	see reference to function template instantiation 'std::binder2nd<_Fn2> 
	std::bind2nd<std::greater<_Ty>,const char[2]>(const _Fn2 &,const char (&))' being compiled
	with
	[
	_Fn2=std::greater<int>,
	_Ty=int
	]
	*/

	// Как видно, отладочное сообщение достаточно громоздкое
}

// ------------------- Мелкое инстанцирование -------------------
// Рассмотрим иерархию вызываемых шаблонных функций

// Эта функция предполагает, что T - указатель!
template<typename T>
void clear(T& p){
	*p = 0; // - ошибка!

}

// Эта просто передает в нее параметр (не обязательно указатель!)
template<typename T>
void core(T& p){
	clear(p);
}

// Эта тоже
template<typename T>
void middle(T& p){
	core(p);
}

// Верхняя функция в иерархии, передает параметр типа int
// Как раз "не указатель"
template<typename T>
void up(const T& env){
	// что-то с env
	typename T::index_t p;
	middle(p);
}

class client{
public:
	typedef int index_t;
};

void test_stack_error(){
	//client cli;	// - раскомментировать для ошибки 1
	//up(cli);		// - раскомментировать для ошибки 1
	// Сообщения об ошибках по всем функциям в стеке вызова
	/*
	error C2100: illegal indirection
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(45) : 
	see reference to function template instantiation 'void clear<T>(T &)' being compiled
	with
	[
	T=client::index_t
	]
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(51) : 
	see reference to function template instantiation 'void core<T>(T &)' being compiled
	with
	[
	T=client::index_t
	]
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(60) : 
	see reference to function template instantiation 'void middle<client::index_t>(T &)' being compiled
	with
	[
	T=client::index_t
	]
	d:\projects\study\cpp\tmpl\part_01_debugging\debug_error.cpp(70) : 
	see reference to function template instantiation 'void up<client>(const T &)' being compiled
	with
	[
	T=client
	]
	*/
}

// Для проверки подобных ситуаций используется модель "мелкого инстанцирования" - 
// на верхнем уровне выполняется проверка шаблонного параметра на соответствие некоторым требованиям
// В данном случае на разыменование указателя. Примерно так:
template <typename T>
class shallow_checks{
public:
	// Пустая функция, подавляющая оптимизацию -
	// иначе параметр не используется
	static void ignore(const T& p){ T a = p; }
	static void check_is_pointer(T ptr){
		ignore(*ptr);
	}
};

template<typename T>
void up1(const T& env){
	// что-то с env
	
	// Инициализируем index_t нулем
	typename T::index_t p();
	
	// Сработает проверка на указатель:
	// shallow_checks<T::index_t>::check_is_pointer(p);// - раскомментировать для ошибки 2
	// error C2100: illegal indirection
	// middle(p);									// - раскомментировать для ошибки 2
}

void test_shallow_check(){
	client cli;
	up1(cli);
}