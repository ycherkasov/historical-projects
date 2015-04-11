#pragma once
#include <iostream>

using std::cout;
using std::endl;

// Здесь хранится класс счетчика ссылок 
// и служебных классов-стратегия для определения деталей поведения умного указателя

size_t* alloc_counter(){
	return new size_t;
}

void free_counter(size_t* c){
	delete c;
}

// Класс счетчика ссылок выделяет память для счетчика вне класса (чтобы класс можно было копировать)
// Могут применяться разные счетчики, например потокобезопасные с блокировками и тп  
class simple_ref_count{
public:

	// примитивный конструктор
	simple_ref_count():_counter(){}

	// конструктор копирования по умолчанию вполне подходит

	// инициализация счетчика (где-то создан объект)
	template <typename T>
	void init(T*){
		_counter = alloc_counter();
		(*_counter) = 1;
		cout << _counter << ", counter = 1" << endl;
	}

	// освобождение счетчика (где-то удалена последняя копия объекта)
	template <typename T>
	void dispose(T*){
		free_counter(_counter);
		cout << _counter << ", dispose counter" << endl;
	}

	// инкремент (где-то скопирован объект)
	template <typename T>
	void increment(T*){
		(*_counter)++;
		cout << _counter << ", counter = " << (*_counter) << endl;
	}

	// декремент (где-то удален объект)
	template <typename T>
	void decrement(T*){
		(*_counter)--;
		cout << _counter << ", counter = " << (*_counter) << endl;
	}

	// проверка на 0
	template <typename T>
	bool is_zero(T*){
		return ((*_counter) == 0);
	}


private:
	size_t* _counter;
};

// Класс стандартной стратегии удаления объекта
struct standard_object_policy{
	template <typename T>
	void dispose(T* ptr){
		delete ptr;
	}
};


// Класс стандартной стратегии удаления массива объектов
struct standard_array_policy{
	template <typename T>
	void dispose(T* ptr){
		delete[] ptr;
	}
};



