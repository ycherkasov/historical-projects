#include "user_allooc.h"
#include "construct.h"

#include <iostream>
#include <new>

using namespace std;

intern g_intern;

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


