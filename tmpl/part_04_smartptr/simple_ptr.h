#pragma once

// Простой умный указатель без копирования и передачи вледения
// аналог scoped_ptr
template <typename T>
class simple_ptr{
public:

	// конструктор по умолчанию
	simple_ptr():_ptr(){}

	// конструктор, принимающий управление памятью
	explicit simple_ptr(T* mem):_ptr(mem){}

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