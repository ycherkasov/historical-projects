#pragma once
#include <iostream>
using std::cout;
using std::endl;
// --- Демонстрация членов шаблонного класса --- 
// 1.Функции-члены
// 2.Вложенные классы
// 3.Статические члены

template <typename T>
class tmpl_members
{
public:
	tmpl_members(const T& v);

	// Обычный метод
	void open();
	
	// Шаблонный метод
	template <typename T2> 
	void copy(const T2& t);
	
	// Статический член
	static double s_dbl;

	// Вложенный класс
	class self{
	public:
		self(int i);
		int _i;
	} _self;

	// Вложенный шаблон
	template <typename T3>
	class self_tmpl{
	public:
		self_tmpl(const T3& t);
		T _t;
	};
	
	self_tmpl<T> _self_tmpl;
private:
	T _val;
};

// Определение консруктора (вызывает конструкторы вложенных классов)
template <typename T>
tmpl_members<T>::tmpl_members(const T& v) : _self(5), _self_tmpl(v), _val(v){}

// Определение обычного метода
template <typename T>
void tmpl_members<T>::open(){

}

// Определение шаблонного метода
template <typename T>
template <typename T2>
void tmpl_members<T>::copy(const T2& t){

}

// Определение статического члена
template <typename T>
double tmpl_members<T>::s_dbl = 0.;

// Определение метода (конструктора) вложенного класса
template <typename T>
tmpl_members<T>::self::self(int i) : _i(i){}

// Определение метода вложенного шаблона
template <typename T>
template <typename T3>
tmpl_members<T>::self_tmpl<T3>::self_tmpl(const T3 &t) : _t(t){}

// --- Демонстрация виртуальных методов в шаблонах --- 
template <typename T>
class base {
public:
	// обычный метод может быть виртуальным в шаблоне
	virtual void open(const T& t){
		cout << "base::open " << typeid(t).name() << endl;		
	}
	// ...а шаблонный - нет
};

template <typename T>
class derived : public base<T> {
public:
	// меотд вызовется виртуально даже для base<double>
	virtual void open(const T& t){
		cout << "derived::open "<< typeid(t).name() << endl;		
	}
};

// ------------------- Связывание шаблонов -------------------

// В отличие от классов, для шаблонов не допускается совпадание имен с другими объектами
// Шаблоны не могут иметь соглашения по вызову extern "C", но могут иметь 
// нестандартные соглашения по вызову extern "X"

// Шаблоны имеют внешнее связывание, кроме объявленных как static
template <typename T>
void external();	// внешнее

template <typename T>
static void internal();	// внутреннее


