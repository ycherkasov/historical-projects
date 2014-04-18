#pragma once
#include <new>

// В этом хедере демонстрируется создание объекта 
// при помощи пользовательского аллокатора

// Мейерс 1 Правило 7
// Можно замещать стандартный обработчик по нехватке памяти
// Стандартно set_new_handler() вызывается, 
// пока память не будет освобождена или программа не завершена
// Пользовательский обработчик может:
// * сделать доступным необходимое количество памяти
// * установить другой обработчик new
// * установить нулевой обработчик (т.е. исключение bad_alloc) или бросить bad_alloc вручную
// * завершить программу по abort()/exit()


// Можно для каждого конкретного класса реализовать свой обработчик set_new_handler()
// Можно создать класс-примесь (mixture) чтобы наследовать от него оператор и обработчик new
// или параметризовать класс им
class NewHandlerSupport{
public:
	static new_handler set_new_handler(new_handler p);
	static void* operator new(size_t size);
	static void operator delete(void* p);
private:
	static new_handler _current_handler;
};


// Мейерс 1 Правило 10
// operator new() и operator delete() перегружаются обычно для повышения эффективности
// это особенно верно для приложений, размещающих много маленьких объектов

// Мейерс 1 Правило 8
// Различайте функцию и оператор new
// Функция просто выделяет память, оператор вызывает конструктор
// Функция delete() и оператор delete() соотносятся примерно так же

class user_allooc
{
public:
	user_allooc();

	// чтобы производные классы корректно работали с operator new
	// деструктор должен быть виртуальным
	virtual ~user_allooc();
	// Мейерс 1 Правило 7
	// переопределения для класса operator new() гарантирует,
	// что для его экземпляров будет вызван именно он
	// Мейерс 1 Правило 8
	// operator new() наследуется производными классами
	// поэтому operator new() в базовом классе нужно проектировать для производных,
	// либо просто вызывать для них стандартный operator new()
	// Также реализуйте родственный operator new[]()
	static void* operator new(size_t s);

	// Без виртуальных деструкторов функция operator delete() может работать ненадлежащим образом
	static void operator delete(void* p);

	// Перегруженная форма замещающего оператора new
	// Мейерс 1 Правило 9
	// В оператор new может быть добавлено сколько угодно параметров
	// Если перегружена какая-либо другая форма new, кроме нормальной
	// (замещающая, с указателем на обработчик),
	// то нормальная обязательно должна быть перегружена
	// Иначе перегруженные операторы сокроют глобальную new()
	static void* operator new(size_t s, void* p);
	
	// pair delete operator for the placement new
	static void operator delete(void *p, void *) {}
	
	static void operator delete(void* p, size_t s);

	// Форма new с замещением new_handler
	static void* operator new(size_t s, new_handler p);


};


// Класс-пул памяти
class memory_pool{
public:
	memory_pool(size_t n);
	void* alloc(size_t n);
	void free(void* p, size_t n);
	~memory_pool();
};

