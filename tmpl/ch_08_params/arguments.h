#pragma once

// --- Аргументы шаблонов функций ---
// Задаются:
//- явно <int, int>
//- именем класса (type_t a; X<a>)
//- по умолчанию
//- вывод аргументов (неявное определение аргументов шаблона, <>)

// Нектоторые аргументы шаблонов функций невозможно получить путем вывода, например:
template <typename DestType, typename SrcType>
inline DestType implicit_cast(SrcType t){
	return static_cast<DestType>(t);
}

// SrcType можно вывести, а DestType - нет
// Можно определить функцию несколько хуже, поменяв местами,
// когда ни один из типов нельзя вывести
template <typename SrcType, typename DestType>
inline DestType implicit_cast2(SrcType t){
	return static_cast<DestType>(t);
}

// При перегрузке шаблонных функций не всгда можно определить конкретную путем подстановки аргументов
// Например, шаблонная функция, принимающая указатель на функцию и ее аргумент
template <typename Func, typename T>
inline void apply(Func fptr, T t){
	fptr(T);
}

// Семейство перегруженных функций
template <typename T>
T multi(T t){
	return t;
}

template <typename T>
T multi(T* t){
	return *t;
}

void test_functions_family(){
	// Раскомментировать для ошибки
	// error C2784: 'void apply(Func,T)' : could not deduce template argument for 'overloaded function type' from 'overloaded function type'
	//apply(multi<int>, 3);
	//apply(multi<int>, 5);
	// Обе перегруженные функции подходят, хотя и введено полное определение типов
}

// ------------------- Аргументы типов -------------------
// Аргументами типов не могут быть:
// 1. Локальные классы и перечисления (объвленные в пределах функции)
// 2. Неименованные классы и перечисления (кроме именованных через typedef)
// Например:
template <typename T>
class List_t{
public:
	List_t() : _t(){}
	List_t(const T& t) : _t(t){}
	T _t;
};

// Неименованная структура с typedef 
// и без нее
struct  {
	double x;
} str1;

typedef struct  {
	double x;
} str2;

// Неименованное перечисление с typedef 
// и без нее
enum {red, green, blue} color1;
typedef enum {red1, green1, blue1} color_t;

void test_argument_types(){
	struct association{
		int* p;
	};
	// Не должно компилироваться, но компилируется
	List_t<association*> err1;

	// Неименованные структуры передавать нельзя
	// а именованные через typedef можно
	//List_t<str1> s1;
	List_t<str2> s1;

	// То же для перечислений
	//List_t<color1> c1;
	List_t<color_t> c2;
}

// Наборы аргументов шаблонов эквивалентны друг другу если 
// они попарно идентичны на этапе компиляции
