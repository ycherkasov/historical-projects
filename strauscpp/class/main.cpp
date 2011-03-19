#include "construct.h"
#include "class.h"
#include "user_allooc.h"
#include "intset_t.h"
#include "word_counter.h"

#include <iostream>

construct g_construct;

// Функция демонстрирует инициализацию и работу 
// с константными объектами и mutable-членами
void show_const(){

	A a;

	bool b = false;
	size_t s = sizeof(b); // 1

	const A aa;
	//aa.set_int1(5); - так нельзя, вызов неконстантного метода в константном классе

	// а так можно, член константного класса mutable может быть изменен
	aa.set_a6_mutable(5);

}

// Функция демонстрирует инициализацию и работу 
// с константными указателями и указателями на константу
void show_const_pointers(){

	// Указатель на константу
	const int* pp1 = new const int(3);

	// Явная ошибка - константа не проинициализирована,
	// но компилятор молчит
	const int* pp2 = new const int;

	// Проинициализировать ее легальным способом теперь не удастся
	// Не сработает даже const_cast
	//const_cast<int>(*pp2) = 1;

	// Можно создать непроинициализированный константный динамический массив
	// Он даже не будет обнулен, и больше с ним ничего сделать нельзя.
	const int* pp3 = new const int[100];

	// Логичнее создавать динамический массив по константному указателю
	int* const pp4 = new int[100];

	// Внимание - Стандарт разрешает динамический массив нулевого размера!
	int* pp5 = new int[0];
	// а статический - не разрешает (хотя есть расширения gcc, где это можно)
	//int p6[0];

	// Функция _msize позволяет узнать реальный размер выделенного массива.
	size_t sz = _msize(pp4);
	sz = _msize(pp5);

	delete pp1;
	delete pp2;
	delete[] pp3;
	delete[] pp4;
	delete[] pp5;
}

// Способы вызова конструктора объекта:
// 1. Именованный автоматический объект(по умолчанию, копия).
// 2. Объект в свободной памяти (new/delete).
// 3. Нестатический объект-член класса.
// 4. Объект - элемент массива.
// 5. Локальный статический объект.
// 6. Глобальный объект.
// 7. Временный объект.
// 8. Объект, сконструированный пользовательским аллокатором.
// 9. Член объединения (union)
void show_construct_destruct(){

	std::cout << std::endl <<  "Create automatic objects" << std::endl;
	// 1. Именованный автоматический объект(по умолчанию, копия).
	// Если в объекте есть константы или ссылки, он не может быть создан 
	// конструктором по умолчанию
	{
		construct c;
		construct d;
		{
			construct c1(c);
		}
		noncopyble nc;
		// А скопировать этот объект нельзя!
		// noncopyble nc1(nc);
	}
	// Создаются в порядке c-d-c1-nc, уничтожаются c1-nc-d-c
	

	// 2. Объект в свободной памяти (new/delete).
	std::cout << std::endl << "Create dynamic objects" << std::endl;

	// Если в классе только автоматически сгенерированный конструктор,
	// то эта форма вызова, с (), обнулит память...
	construct* pc1 = new construct();
	// ... а эта - нет
	construct* pc2 = new construct;

	// Обнуляйте неиспользуемые указатели!
	delete pc1; pc1 = 0;
	delete pc2; pc2 = 0;

	// 3. Нестатический объект-член класса.
	std::cout << std::endl << "Create non-static member" << std::endl;
	// Вызовется конструктор по умолчанию intern()
	{
		construct c1;
	}
	

	// 4. Объект - элемент массива.
	std::cout << std::endl << "Create objects array " << std::endl;
	// ! Элементы массива могут быть сконструированы по умолчанию
	construct arr_c[5];
	// Можно использовать список инициализации в виде конструкторов
	construct arr_1[] = { construct(2), construct( g_construct ) };
	// Можно использовать даже неявные преобразования при конструировании
	construct arr_2[] = {1, 2};
	// прим.-такие массивы лучше объявлять без указания количества элементов - []

	// 5. Локальный статический объект.
	std::cout << std::endl << "Create local static objects " << std::endl;
	create_static(0);
	create_static(1);
	create_static(2);

	// В случае простых объектов можно пользоваться представлением в виде функций
	int st = return_static();

	// 6. Глобальный объект.
	// Конструкторы глобальных объектов вызываются в одном модуле - в порядке определения
	// в разных модулях - порядок не определен. То же для разрушения объектов (см. g_construct, g_intern)
	// Полезно использовать синглтон, а также обертку-инициализатор (см. construct.h)

	// 7. Временный объект.
	// Убьется сразу
	std::cout << std::endl << "Create temporary object " << std::endl;
	construct(5);
	// Также неявное копирование происходит при передаче и возвращении по значению 
	// и бросании исключения

	// 8. Объект, сконструированный пользовательским аллокатором.
	{
		void* buf = reinterpret_cast<void*>(0xF00F);

		// использование замещающего оператора new
		// объект будет создан по адресу buf
		user_allooc* ua = new(buf)user_allooc;
		delete ua;
	}
	

	// 9. Член объединения (union)
	// union не может иметь членов с конструкторами-деструкторами
	// потому что непонятно какой из них вызывать
}

void bool_logics(){

	// Демонстрирует упражнение с реализацией булевых операций на множестве
	std::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);

	std::vector<int> v2;
	v2.push_back(3);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);

	intset_t set1(v1);
	intset_t set2(v2);

	intset_t set_sum = set1 + set2;
	set_sum = set1 - set2;
	set_sum = set1 * set2;
}

int main(){

	word_counter w;
	std::string s( "test.txt" );
	w.count_from_file( s );

	show_const_pointers();

	A a(1,1,1);
	int aa = a.get_int6();
	show_const();
	show_construct_destruct();
	
	// Статический метод можно вызывать как для класса, так и для объекта
	construct::out_static_array();
	construct c;
	c.out_static_array();

	// Демонстрация перегруженных операторов new
	show_new1();
	show_new2();
	show_new_delete();
	bool_logics();


	return 0;
}
