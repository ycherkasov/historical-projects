#include <iostream>
#include <typeinfo>
#include "tuple.h"

using namespace std;

// Создание кортежей путем рекурсивного вложения в pair
void show_recursive_pair(){
	
	// Можно создавать кортежи рекурсивным вложением в Pair
	typedef Pair<int, Pair<char, Pair<double, long> > > tuple4;
	// <int, char, double, long>

	// Извлечем 2 и 3 типы
	typedef recursive_tuple<4, tuple4>::Result type4;
	typedef recursive_tuple<3, tuple4>::Result type3;
	typedef recursive_tuple<2, tuple4>::Result type2;
	typedef recursive_tuple<1, tuple4>::Result type1;
	cout << "Type 1 is " << typeid(type1).name() << endl;
	cout << "Type 2 is " << typeid(type2).name() << endl;
	cout << "Type 3 is "<< typeid(type3).name() << endl;
	cout << "Type 4 is "<< typeid(type4).name() << endl;

	// назначим значения кортежу
	tuple4 a;
	value<1>(a) = 1;
	value<2>(a) = 'a';
	value<3>(a) = 1.0;
	value<4>(a) = 10;
}

void show_tuple(){
	Tuple<bool> t1 = make_tuple(true);
	Tuple<bool, int> t2 = make_tuple(true, 10);
	Tuple<bool, int, char> t3 = make_tuple(true, 10, 'a');
	Tuple<bool, int, char, long> t4 = make_tuple(true, 10, 'a', static_cast<long>(-1000));
	Tuple<bool, int, char, long, unsigned> t5 = make_tuple(true, 10, 'a'
		, static_cast<long>(-1000)
		, static_cast<unsigned>(1));
}

int main(){

	show_recursive_pair();
	show_tuple();

	return 0;
}

