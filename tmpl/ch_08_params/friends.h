#pragma once

// Ўаблоны и друзь€
// 1.ќбъ€вление дружественной функции может быть единстренным объ€влением объекта 
// (т.е. кроме как в классе нигде не видна)
// 2.ќбъ€вление дружественной функции м.б. определением.


// - ќбъ€вление дружественных классов не может быть определением.
// - ћожно объ€вл€ть дружественным конкретный экземпл€р шаблона.
// - ƒружественный шаблон должен быть видим в точке объ€влени€, в отличие от класса

// ѕримеры дружественных функций:

// ѕерегрузка функции и шаблона
void comrade(void* ){}

template <typename T>
void comrade(T){}

template <typename T>
class comrade_t;

class comrades{
public:
	// ќбъ€вление друга совпадает с определением
	friend void comrade(int){}

	// дружественна€ функци€
	friend void comrade(void*);

	// дружественна€ шаблонна€ функци€ (выведение аргумента)
	friend void comrade(int);

	// дружественна€ шаблонна€ функци€ (€вное указание аргумента)
	friend void comrade<double>(double);

	// ќбъ€вление дружественной шаблонной функции совпадает с определением
	template <typename T>
	friend T comrade(const T t){
		return t;
	}

	// ќбъ€вление дружественного класса
	template <typename T>
	friend class comrade_t;
};

// ¬озможно определение дружественного шаблона и экземпл€ра шаблона
template <typename T>
class Node;

template <typename T>
class Node2;


template <typename T>
class Tree{
	friend class Node<T>;
	friend class Node2<int>;
};

// ¬ажно - в шаблонном классе нельз€ определ€ть дружественную функцию,
// иначе это вызовет нарушение ODR

template<typename T>
class createor{
public:
	createor(){}

	// –аскомментировать дл€ ошибки:
	// error C2084: function 'void createor<T>::appear(void)' already has a body
	// friend void appear(){ int i = 1; }

	// —ледует дружественную функцию генерировать в зависимости от типа
	friend void appear( createor<T>* ){ int i = 1; }
};

// ƒважды определ€етс€ дружественна€ функци€!
void test_friends_odr(){
	createor<int> a;
	createor<double> b;
}