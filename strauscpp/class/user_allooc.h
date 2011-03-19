#pragma once

// В этом хедере демонстрируется создание объекта 
// при помощи пользовательского аллокатора

void show_new1();
void show_new2();
void show_new_delete(); 

class user_allooc
{
public:
	user_allooc();
	~user_allooc();
	void* operator new(size_t s, void* p);
	void operator delete(void* p);
};

