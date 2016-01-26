#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <exception>
#include "multitree.h"

using namespace std;

class checker{
public:
	checker():_counter(){}
	~checker(){

		if(_counter != 0){
			cout << "Wrong sequence" << endl;
		}
		else{
			cout << "Right sequence" << endl;
		}
	}
	void checked_push(const char s){
		if( s == '('){
			_counter++;
		}
		if(s == ')'){
			_counter--;
			if(_counter < 0)
				return;
		}
	}

	void check_braces(const string& s){
		string::const_iterator s_it = s.begin();
		stack<char> strst;
		while(s_it != s.end()){
			checked_push(*s_it);
			++s_it;
		}
	}

	unsigned _counter;
};

struct node{
	node(int v):val(v),next(),rand(){}
	int val;
	node* next;
	node* rand;
};

struct double_list{
	double_list():count(),start(),last(){}
	void add(int v){
		if(start == 0){
			start = new node(v);
			last = start;
			count++;
		}
		else{
			node* tmp = new node(v);
			last->next = tmp;
			last = tmp;
			count++;
		}
	}
	void randomize(){

	}
	unsigned count;
	node* start;
	node* last;
};

void braces(const string& s){
	checker c;
	string mystr(s);
	c.check_braces(mystr);
}

void list_copy(){
	double_list l;
	l.add(1);
	l.add(2);
	l.add(3);
	l.add(4);
}

// dynamics - maximum subsequense finishes in element j
void growing_subsequence_count(){
	int a[] = {4,5,7,2,1,5,18,8};
	// output 1,2,3,1,1,2,4,1
	gsq(a, sizeof(a))
}


void gsq(int a[], size_t n){
	std::vector<int> d;
	std::vector<int> prev;
	d.reserve(n);
	prev.reserve(n);
	for(int i = 2 ; i < 7 ; i++)
	//d[i] = 1 + max( d[j]: >= i ), a[j] , a[i]
	return *(max(d.begin(), d.end()));
}

// Edit distance (Levenshtein distance)
// http://ru.wikipedia.org/wiki/Расстояние_Левенштейна
void edit_distance(){

}

struct item{
	item(unsigned weight, unsigned cost):_cost(cost), _weight(weight){}
	unsigned _weight;
	unsigned _cost;
};

struct backpack{
	backpack(unsigned volume):_volume(volume),_total(){}
	const unsigned _volume;
	std::vector<item> _items;
	unsigned _total;
	void push(const item& i){
		if((_total + i._weight) >= _volume){
			_items.push_back(i);
			_total += i._weight;
		}
	}
};

void test_backpack(){
	backpack bp(10);
	std::vector<item> v_item;
	
	// c,w
	v_item.push_back(item(5,3)); 
	v_item.push_back(item(2,7));
	v_item.push_back(item(10,4));
	v_item.push_back(item(1,2));
	
	//todo : init weights+1/cost+1 matrix
	unsigned wm[11][5] = {};

	// todo :
	//unsigned max_weight = 

	//


	// оптимально заполнить рюкзак, имея i-1 предметов или
	// w-1 рюкзак

	int item_no = 1;
	std::vector<item>::iterator it = v_item.begin();
	while(it != v_item.end()){
		// идем до веса i-го предмета
		for(unsigned cur_weight = 1 ; cur_weight <= 10 ; i++){
			// максимум - оставить или выкинуть
			if(i == (*it)._weight)
				wm[cur_weight][item_no] = 
				// todo : check it!!
					std::max(  wm[cur_weight][item_no-1], wm[cur_weight - (*it)._weight][item_no - 1] + (*it)._cost  );
			++it; ++item_no;
		}
	}
	// FPTAS
	// рекурсивное ленивое вычисление рез-та

	// Динамическое прогрп=аммирование - обычно граф
	// Время пропорцонально кол-ву ребер
	// память - колву вершин

	// можно экономить память, сохраняя только необходимые столбцы

}

// последовательное перемножение матриц
// [pq] [qn] = [pn]
/*

(A1*A2)*A3
A1 2*50
A2 50*2
A3 2*50

2*50*2 + 2*2*50 = 400

OR
A1*(A2*A3)
(50*2*50)+(50*2*50)=10000

завести массив стоимостей перемножения матриц D[i][j]
*/

void matrix_cost(){
	int D[5][5] = {};
	for(int i = 0; i < 5 ; i++) D[i][i] = 0;
	// далее двигаться по диагонали снизу 
	//http://ru.wikipedia.org/wiki/Задача_о_порядке_перемножения_матриц

}

/*
Задача на деревьях
Независимые вершины - несовмещенные одним ребром
Найти max множество
Вести динамику по поддеревьям
Может сработать жадный алгоритм
*/

/*
n-угольник, оптимальная триангуляция
стоимость триангуляции - суммарная длина всех диагоналей
*/

// поиск маскимальной подпоследовательности
void max_subeq(){
	int a[10] = {2,5,7,4,-7,9,0,6,-5,-4};
	int d[10] = {};
	for(int i = 0 ; i < 10 ; i++) d[i] = std::max( a[i], d[i+1] + a[i] );
}

// задача о размене монет

void triangle(){
	// sum of minimal
}

int main(int argc, char* argv[]){
	//braces(argv[1]);
	return 0;
}