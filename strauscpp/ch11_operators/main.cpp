#include "complex.h"
using std::vector;

// Функция демонстрирует работу перегруженных операторов 
// и неявного конструктора
void show_overloads(){

	// Вызов неявного конструктора
	complex_t c1 = 3;

	// Явные вызовы конструктора
	complex_t c2(1.,2.5);
	complex_t c3(1.5,3.5);

	c1 = c2 + c3;
	c1 = c2 - c3;
	c1 = c2 * c3;
	c1 = c2 / c3;

	c1 += c2;
	c2 -= c3;
	c1 *= c3;
	c3 /= c1;
}

// Функция демонстрирует перегрузку опрераторов к классе,
// использующем динамическую память
void show_dynamic(){
	
	complex_matrix m1(2);
	complex_matrix m2(2,2);

	m1(0,0) = complex_t(1.,2.);
	m1(0,1) = complex_t(3.,5.2);
	complex_t* pc = m2[1];

}

void gimme_vector_by_val(vector<int> v){
	size_t s = sizeof v;
	bool b = v.empty();
}

void show_vector_by_val(){
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	size_t s = sizeof v;
	gimme_vector_by_val(v);
}

int main(){

	show_vector_by_val();
	show_dynamic();
	show_overloads();
	return 0;
}