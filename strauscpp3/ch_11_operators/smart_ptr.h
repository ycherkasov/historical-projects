#pragma once

// В самописных smart ptrs можно приводить тип одного указателя к другому с помощью 
// шаблонного оператора приведения типа (Мейерс 2-28)
template <typename T>
class simple_ptr{
public:

	// конструктор, принимающий управление памятью
	explicit simple_ptr(T* mem = nullptr) : _ptr(mem){}

	// оператор преобразования simple_ptr в
	// simple_ptr любого типа! (конечно если указатель приводится к нему)
	// Достаточно опасный подход, здесь приведен для примера
	template <typename NewType>
	operator simple_ptr<NewType>(){
		// return 'new' value because new pointer should own a new copy
		// so that destroy it (otherwise it should be something like refcount)
		return simple_ptr<NewType>(new T(*_ptr));
	}

	// освобождение памяти
	~simple_ptr(){
		delete _ptr;
	}

	// Операции указателя
	T* operator->() const {
		return _ptr;
	};

	T& operator*() const {
		return *_ptr;
	};

	// функциональный аналог оператора ->
	T* get() const {
		return _ptr;
	};

	// освобождение от владения
	T* release() {
		T* ptr = _ptr;
		_ptr = 0;
		return ptr;
	};

private:
	T* _ptr;
};



