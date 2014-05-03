#pragma once

#include <utility>
#include <iterator>


// Класс итератора, бросающий исключения при выходе за границы контейнера
// Что-то подобное применяется в отладочных версиях

// Нигде не сказано, что новый итератор должен наследоват от итератора
// Поэтому будем наследовать непосредственно от свойств

template <typename Container, typename Iter = typename Container::iterator>
class Checked_iterator : public std::iterator_traits<Iter>{
public:

	// Валидация существующего итератора из коструктора
	void valid(Iter p) const {

		if (cont->end() == p){
			return;
		}

		for(Iter pp = cont->begin() : pp != cont->end(); ++p){
			if(pp == p)
				return;
		}

		throw_oor();
	}

	// Коммутативная проверка на равенство
	friend bool operator == (const Checked_iterator& a, const Checked_iterator& b){
		return ( (a.cont == b.cont) && (a.iter == b.iter) );
	}

	// Инициализация контейнером и итератором
	Checked_iterator(Container& c, Iter p): cont(&c), iter(p){
		valid(p);
	}

	// Инициализация контейнером и указателем на его начало
	Checked_iterator(Container& c): cont(&c), iter(c.begin()){}

	// Функции итератора - operator* 
	reference operator*() const {
		if(iter == c->end()){
			throw std::out_of_range("OOR");
		}
		return iter;
	}

	// Функции итератора - operator->
	reference operator->() const {
		// проверяется оператором *
		return &*iter;
	}

	// Функции итератора - operator+
	Checked_iterator operator+( difference_type d ) const {
		if(  
			( ( c->end() - iter) < d  ) || // если при сложении уезжаем за конец
			(d < -(iter  - c->begin()) ) ){ // или до начала

				throw std::out_of_range("OOR");	// исключение
		}
		// Создать итератор
		return Checked_iterator(cont, iter+d);
	}

	// Функции итератора - operator[]
	reference operator[]( difference_type d ) const {
		// если при индексации уезжаем за конец или хотя бы достигаем его
		if( ( ( cont->end() - iter) <= d  ) || 
			// или до начала
			(d < -(iter  - cont->begin()) ) ){ 
				throw_oor();
		}
		// Создать итератор
		return iter[d];
	}

	// Функции итератора - operator++
	Checked_iterator& operator++(  ) {

		if(iter == cont->end()){
			throw_oor();
		}
		++iter;
		return *this;
	}

	// Функции итератора - operator++(int)
	Checked_iterator operator++( int ) {
		Checked_iterator temp = *this;
		++(*this);
		return temp;
	}

	// Функции итератора - operator--
	Checked_iterator& operator--(  ) {

		if(iter == cont->begin()){
			throw_oor();
		}
		--iter;
		return *this;

	}

	// Функции итератора - operator--(int)
	Checked_iterator operator--( int ) {
		Checked_iterator temp = *this;
		--(*this);
		return temp;
	}


	// Обращенеи по индексу - для RA
	difference_type index() const {
		return iter - c.begin();
	}

	// Получение базового итератора
	Iter unchecked() const {
		return iter;
	}
protected:

	// Это исключение кидается при выходе за границы - 
	// это и делает итератор "проверяемым"
	void throw_oor() const {
		throw std::out_of_range("OOR");
	}


private:
	// Базовый итератор без проверки
	Iter iter;

	// Базовый контейнер
	Container* cont;
};

// Т.к. мы не можем внедрить наш итрератор с проверкой в сторонние контейнеры,
// мы можем смоделировать конейнеры для них наследованием

template <typename C>
class Checked_container : public C{
public:

	typedef Checked_iterator<C> iterator;
	typedef Checked_iterator<C, typename C::const_iterator> const_iterator;

	// контейнер по умолчанию
	Checked_container():C(){}

	// контейнер с указанием размера
	explicit Checked_container(size_t n):C(n){}

	// Функции доступа к началу и концу
	iterator begin(){
		return iterator(*this, C::begin());
	}

	const_iterator begin() const {
		return const_iterator(*this, C::begin());
	}

	iterator end(){
		return iterator(*this, C::end());
	}

	const_iterator end() const {
		return const_iterator(*this, C::begin());
	}

	//  []
	typename C::reference operator[] (typename C::size_type n){
		return Checked_iterator<C> (*this) [n];
	}

	//  получить базовый контейнер
	C& base(){
		return this;
	};

};
