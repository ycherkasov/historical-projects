#pragma once
#include "ref_counter.h"

// Класс умного указателя со счетчиком закрыто наследует от классов стратегий
// определяющих место хранения счетчика и процедуру уделения объекта
template <typename T,
	typename ref_count_policy = simple_ref_count,
	typename deletion_policy = standard_object_policy
>
class counting_ptr : ref_count_policy, deletion_policy{
	typedef ref_count_policy RCP;
	typedef deletion_policy DP;
public:
	// конструктор по умолчанию
	counting_ptr():_ptr(){}

	// конструктор, принимающий управление памятью и создающий счетчик
	explicit counting_ptr(T* mem):_ptr(mem){
		this->init(mem);
	}

	// копирующий конструктор, увеличивающий значение счетчика
	counting_ptr(const counting_ptr& rhs)
		: ref_count_policy(rhs)
		, deletion_policy(rhs){
		
			this->attach(rhs);
	}

	// деструктор уменьшает значение счетчика
	~counting_ptr(){
		this->detach();
	}

	// оператор присвоения с проверкой на this
	// увеличивает значение счетчика для привсаиваемого объекта
	// уменьшает для себя
	counting_ptr& operator=(const counting_ptr& rhs){
		if(this->_ptr != rhs._ptr){
			// уменьшение счетчика для себя
			this->detach();

			// копирование данных и указателя
			ref_count_policy::operator=(rhs);
			deletion_policy::operator=(rhs);
			this->attach(rhs);
		}
		return (*this);
	}

	// Операции указателя (константные)
	inline const T* operator->() const {
		return _ptr;
	};

	inline const T& operator*() const {
		return *_ptr;
	};

	// Операции указателя (неконстантные)
	inline T* operator->() {
		// неконстантная операция! отсоединяем объект, создаем новый
		this->detach();
		this->init(this->clone());
		return _ptr;
	};

	inline T& operator*() {
		// неконстантная операция! отсоединяем объект, создаем новый
		this->detach();
		this->init(this->clone());
		return *_ptr;
	};

private:
	// защищенные функции-хелперы

	// инициализация указателем
	void init(T* p){
		if(p){
			// тут буджет создан счетчик, 
			// в общем случае можем не знать каким образом
			ref_count_policy::init(p);
		}
		this->_ptr = p;
	}

	// копирование указателя и инкремент счетчика
	void attach( counting_ptr<T, RCP, DP> const& rhs ){
		this->_ptr = rhs._ptr;
		if(rhs._ptr){
			ref_count_policy::increment(rhs._ptr);
		}
	}

	// уменьшения счетчика и освобождение если  == 0
	void detach(){
		if(this->_ptr){
			ref_count_policy::decrement(this->_ptr);

			// Проверка на 0 - очищаем счетчик и удаляем сам объект
			if(ref_count_policy::is_zero(this->_ptr)){
				ref_count_policy::dispose(this->_ptr);
				deletion_policy::dispose(this->_ptr);
			}
		}
	}

	// копируем объект при отсоединении
	T* clone(){
		return new T(*_ptr);
	}

private:
	T* _ptr;
};