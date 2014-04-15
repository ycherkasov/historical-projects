#include "user_allooc.h"
#include "construct.h"

#include <iostream>
#include <cstdio>
#include <new>

using namespace std;

intern g_intern;

// new выделяет память, вызывает конструктор, возвращает указатель нужного типа.
// placement new убирает первый шаг, оставляя вызов конструктора и возврат указателя нужного типа.
// Если ты убираешь и конструктор, остается просто возврат указателя — это reinterpret_cast

/** @brief 
3 формы оператора new 
1 - обычная(show_new1)
2 - с замещением(show_new2 - i и j имеют одинаковые адреса)
*/

void show_new1() {
	// просто выделяем память
	int* i = new int(6);
	printf("%d\n", *i);
	delete i;
}


void show_new2() {
	int * i = new int(12);
	printf("*i = %d\n", *i);

	// инициализируем существующую память, в данном случае i = j
	int * j = new(i) int(7);   // замещающий new
	printf("*j = %d\n", *j);
	printf("*i = %d\n", *i);
	delete i;   // сразу же удалили и j
}

user_allooc::user_allooc(void)
{
	cout << "user_allooc()" << endl;
}

user_allooc::~user_allooc(void)
{
	cout << "~user_allooc()" << endl;
}

void* user_allooc::operator new(size_t s) {
	
	void* p = 0;
	// При реализации оператора new надо придерживаться нескольких простых правил
	// 1. Проверка на 0
	// Стандартный трюк - подменить 0 на 1 или выйти
	if(0 == s){
		s = 1;
	}

	// 2. Обработка ситуации нехватки памяти
	while(true){
		p = alloca(s);
		if(p){
			return p;
		}
		else{
			// set_new_handler defines what to do in case of
			// lack of memory
			new_handler globalHandler = set_new_handler(0);
			if(0 == globalHandler){
				// не вызывать обработчик, просто кидать исключение
				throw bad_alloc();
			}
			else{

				set_new_handler(globalHandler);
				globalHandler();
			}

		}
	}

	return p;
}

void user_allooc::operator delete(void* p){
	// Мейерс 1 Правило 8
	// Для operator delete() необходимо гарантировать проверку на ноль
	if(0 == p)
		return;
	free(p);
}

// Перегруженная форма замещающего оператора new
void* user_allooc::operator new(size_t s, void* p){
	return p;
}

void user_allooc::operator delete(void* p, size_t s){
}

// Форма new с замещением new_handler
void* user_allooc::operator new(size_t s, new_handler handler){
	new_handler oldHandler = set_new_handler(handler);
	void* p = alloca(s);
	if(p){
		return p;
	}
	// в случае неудачи будет вызван новый обработчик
	return nullptr;
}

// new_op_new.cpp
// compile with: /EHsc

using namespace std;

class MyClass 
{
public: 
	MyClass( ) : imember()
	{
		cout << "Construction MyClass." << this << endl;
	};

	~MyClass( )
	{
		imember = 0; cout << "Destructing MyClass." << this << endl;
	};
	int imember;
};

void show_new_delete()
{
	// первая, классическая форма оператора new
	// В случае неудачи кидает исключение bad_alloc
	MyClass* fPtr = new MyClass;
	delete fPtr;

	// вторая, замещающая функция new
	// помещает объект по определенному адресу
	// Не требует освобождения памяти
	// Используется когда для переинициализации объекта используются конструкторы. 
	// Обычно необходимо при работе с классами которые нельзя изменить.
	char x[sizeof( MyClass )];
	MyClass* fPtr2 = new( x ) MyClass;
	fPtr2->~MyClass();
	cout << "The address of x[0] is : " << ( void* )&x[0] << endl;
	// http://rsdn.ru/forum/cpp/3679187.aspx

	//Интенсивно применяется в микроконтроллерных системах, 
	//где нужен четкий контроль за размещением объектов в памяти. 
	//Там, зачастую, кучу использовать нельзя, т.к. нужно на момент компиляции знать, 
	//сколько памяти использовано, а сколько еще доступно.

/*	В слабых микроконтроллерах чаще применяют размещающие операторы new, 
	а в более мощных можно уже воспользоваться перегруженными версиями операторов new/delete.
	Проблема в том, что операторы new/delete многое не знают. 
	Например, при работе с мелкими объектами overhead может быть слишком большим, 
	либо за счет фрагментации памяти (т.к. объекты разных размеров) 
	может падать производительность или возрастать расход памяти. 
	Если же вы знаете, что у вас создается только два типа объекта 
	(и вы знаете их максимальное число, что для микроконтроллерных проектов является нормой),
	то по старту приложения вы можете выделить один большой кусок памяти 
	(или два больших куска, каждый для своего типа объектов) для всех таких объектов, 
	и перегрузить оператор new/delete для работы уже с
	собственной выделенной "подкучей". 
	Таким способом вы можете контролировать расход памяти и здорово поднять производительность.
	*/

	// третья, устаревшая форма оператора new
	// В случае неудачи возвращает 0
	MyClass* fPtr3 = new( nothrow ) MyClass;
	if(fPtr3 == 0){
		/*error*/ 
	}
	delete fPtr3;
}


void no_more_memory(){
	cout << "no more memory: new_handler" << endl;
	// Мейерс 1 Правило 8
	//На самом деле operator new() пытается выделить память более одного раза,
	//предполагая, что функция-обработчик в промежутках что-то попытается сделать для ее освобождения

	// Обработчик вызывается непрерывно, поэтому он должен делать следующее
	// 1. попытаться освободить память
	// 2. выйти из программы
	// 3. установить обработчик set_new_handler()
	// 4. установить set_new_handler(0) - тогда будет просто брошено исклучение bad_alloc

	// exit вызовет деструкторы!
	exit(1);
}

// Mixtury class defenition that replaces standard
// new_handler
new_handler NewHandlerSupport::_current_handler = 0;

new_handler NewHandlerSupport::set_new_handler(new_handler p)
{
	new_handler old_handler = _current_handler;
	_current_handler = p;
	return old_handler;
}

void* NewHandlerSupport::operator new(size_t sz)
{
	new_handler global_handler = std::set_new_handler(_current_handler);
	void* mem = nullptr;

	try{
		mem = ::operator new(sz);
	}
	catch(const std::bad_alloc&){
		set_new_handler(global_handler);
		throw;
	}
	set_new_handler(global_handler);
	return mem;
}

void NewHandlerSupport::operator delete(void* p){
	::operator delete(p);
}

void allocate_a_lot(){
	size_t large_memory = 4000000000;
	long* l = new long[large_memory];
}

void show_bad_alloc(){
	try{
		allocate_a_lot();
	}
	catch(bad_alloc&){
		cout << "no more memory: bad_alloc handler" << endl;
	}

	set_new_handler(no_more_memory);
	allocate_a_lot();
}

