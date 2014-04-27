#pragma once

// ¬ самописных smart ptrs можно приводить тип одного указател€ к другому с помощью 
// шаблонного оператора приведени€ типа (ћейерс 2-28)
template <typename T>
class simple_ptr{
public:

	// конструктор, принимающий управление пам€тью
	explicit simple_ptr(T* mem = nullptr) :_ptr(mem){}

	// оператор преобразовани€ simple_ptr в
	// simple_ptr любого типа! (конечно если указатель приводитс€ к нему)
	// ƒостаточно опасный подход, здесь приведен дл€ примера
	template <typename NewType>
	operator simple_ptr<NewType>(){
		return simple_ptr<NewType>(_ptr);
	}

	// освобождение пам€ти
	~simple_ptr(){
		delete _ptr;
	}

	// ќперации указател€
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

	// освобождение от владени€
	T* release() {
		T* ptr = _ptr;
		_ptr = 0;
		return ptr;
	};

private:
	T* _ptr;
};



