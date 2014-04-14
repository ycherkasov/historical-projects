#include <iostream>
#include <list>
#include <vector>
#include "checked_iterator.h"
#include "custom_allocator.h"

using namespace std;

void show_iterator_traits(){
	// Итератор - это абстракция (то, что ведет себя как итератор - это итератор)
	// Чаще всего это указатель или объект

	// Виды итераторов:
	// Output
	// Input
	// Forward
	// Bidirectional
	// Random-access
	int a[] = {0,1,9,3,5,4,2,7,6,5};

	// вектор имеет Random Access итераторы
	vector<int> v1(a, a + sizeof(a)/sizeof(int));

	// а список имеет Bidirectional
	list<int> l1(a, a + sizeof(a)/sizeof(int));

	// Расстояние между итераторами
	// Для вектора
	typedef vector<int>::const_iterator::difference_type vect_int_dist;
	vect_int_dist vdist = distance(v1.begin(), v1.end());

	// Для списка
	typedef list<int>::const_iterator::difference_type list_int_dist;
	list_int_dist ldist = distance(l1.begin(), l1.end());

	// в подобных алгоритмах также неявно передается объект "категории итератора"
	// он позволяет выбрать перегрузку, соответствующую типу итератора
	// Например, для RA это просто разность,
	// а для BI это пробежка от одного итератора до другого

	// Аналогично с операцией advance (аналог +=)
	// += 5 (то же что и operator[])
	advance(v1.begin(), 5);

	// пробежка по списку (аналог operator[] для std::list)
	advance(l1.begin(), 5);

	// выделяют также операторы-вставки
	// и обратные итераторы (рассмотрены в 18)
	// А также итераторы потоков (рассмотрены в 03)
}

void show_checked_iterator(){

	// Конетйнеры с проверкой 
	Checked_container< vector<int> > v1;
	Checked_container< vector<int> > v2(10);

	Checked_container< list<int> > l1;
	Checked_container< list<int> > l2(10);

	// Итераторы с проверкой 
	try{
		// инициализация v2.begin()
		Checked_container< vector<int> >::iterator it(v2);
		while(true) it++;
	}
	catch(const std::exception& e){
		cout << "exception: " << e.what() << endl;
	}

}

void show_custom_allocator(){

}

int main(){

	show_iterator_traits();
	show_checked_iterator();
	show_custom_allocator();

	return 0;
}