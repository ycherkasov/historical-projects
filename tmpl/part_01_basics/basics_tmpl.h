#include <iostream>
#include <bitset>
using std::cout;
using std::endl;
// Глава 5 Джосаттиса
// Ключевое слово typename. Конструкция X.template.
// Использование this-> 
// Шаблоны - члены классов. Шаблонные параметры.
// Инициализация нулем. Использование литералов.

// ------------------- Ключевое слово typename -------------------
// Ключевое слово typename используется не только для указания параметров-типов
// Но и для указания, что данная конструкция, производная от параметра, является типом

// Функция выводит элементы произвольного контейнера типа T
template <typename T>
void print_container(const T& coll){

	// Укажем, что итераторы - тоже типы
	// Применение ключевого слова typename к итератору - стандартный прием
	typename T::const_iterator pos;
	typename T::const_iterator end(coll.end());

	// Итерирование
	pos = coll.begin();
	while( pos != end ){
		std::cout << *pos << endl;
		++pos;
	}
}


// ------------------- Ключевое слово template -------------------
template<int N>
void print_bitset(const std::bitset<N>& b ){

	// Нужно уточнить, что параметр b зависит от шаблона (по Стандарту необходимо,
	cout << b.template to_string<char, char_traits<char>, allocator<char> >() << endl;	

	// ...но в некоторых компиляторах необязательно)
	cout << b.to_string<char, char_traits<char>, allocator<char> >() << endl;	
}

void test_bitset(){
	bitset<10> b;
	print_bitset(b);
}

// ------------------- Использование this -------------------
template <typename T>
class base{
public:
	void exit(){
		cout << "base::exit" << endl;
	}
};

template <typename T>
class derived : public base<T>{
public:
	void foo(){
		
		// Не надо:
		// exit();

		// Никогда не не рассматривает exit() из класса base()

/*
Запись this->exit() заставляет компиляторы думать, что "у данного класса должен быть член exit". 
Поскольку это долженствование не определено до самого момента воплощения (мало ли, как будет специализирован 
Base для конкретных параметров), весь остальной смысловой разбор откладывается на потом.
Аналогичные намёки — это Derived::exit() и Base::exit().

В противном случае gcc пытается с самого начала найти имя exit в текущем контексте. А текущий контекст — это
— тело функции Derived<T>::g()
— объявление класса Derived<T> за исключением белого пятна в виде базы Base<T>
— пространство имён ::, в котором объявлен Derived
Такое имя там есть. Это extern "C" void exit(int exitcode) из <stdlib.h>
*/
		// Надо
		this->exit();
		// или так
		//Base<T>::exit();
		
		// или так
		//using Base<T>::exit;    
		//exit();

		// Вообще в случае наследования шаблонов лучше писать this->member явно
		// Это необходимо в том числе для достижения виртуальности!
	}
};


// ------------------- Шаблоны-члены классов -------------------
// Шаблонный метод или класс м.б. членом класса (как шаблонного, так и обычного)

// Объявление
template <typename T>
class copyable_stack{
public:
	copyable_stack();
	void push(const T& p);
	void pop();
	const T& top() const;
	bool empty() const;

	// Щаблонный метод
	template<typename TCOPY>
	copyable_stack<T>& operator=(const copyable_stack<TCOPY>& s);

private:
	std::vector<T> _copyable_stack;
};

template <typename T>
copyable_stack<T>::copyable_stack(){}

template <typename T>
void copyable_stack<T>::push(const T &p){
	_copyable_stack.push_back(p);
}

template <typename T>
void copyable_stack<T>::pop(){
	_copyable_stack.pop_back();
}

template <typename T>
const T& copyable_stack<T>::top() const {
	return _copyable_stack.back();
}

template <typename T>
bool copyable_stack<T>::empty() const{
	return _copyable_stack.empty();
}

// Инстанс шаблонного класса стека не может быть скопирован 
// инстансу другого шаблонного типа
// например stack<int> a; stack<double> b; a=b

// Для этого можно переопределить в классе copyable_stack шаблонный operator=
// определения внутреннего и внешнего шаблона описываются раздельно
template <typename T>
template <typename TCOPY>
copyable_stack<T>& copyable_stack<T>::operator=( const copyable_stack<TCOPY>& s ) {
	
	copyable_stack<TCOPY>  tmp(s);
	_copyable_stack.clear();

	// Копируем почленно с приведением типа (чтобы проверялось в момент компиляции)
	while( !tmp.empty() ){
		_copyable_stack.push_back( static_cast<T>( tmp.top() ) );
		tmp.pop();
	}
	return *this;
}

// ------------------- Шаблонные параметры шаблонов -------------------
// Параметром шаблона может быть шаблон
// Например, стек с кастомным контейнером может принимать полностью определение 
// контейнера, на котором основан стек, примерно так:
template < 
	typename T, 
	// Т.к. CONT определяет имя класса, а не тип, используется ключевое слово class
	// Имена неиспользуемых параметров можно опустить (... typename = std::allocator)
	template <typename ELEM, typename = std::allocator<ELEM> > class CONT = std::deque >
class tmpl_stack {
public:
	tmpl_stack();
	void push(const T& p);
	void pop();
	const T& top() const;
	bool empty() const;

	// Щаблонный метод
	template<typename TCOPY,template <typename, typename> class CONT_COPY>
	tmpl_stack<T,CONT>& operator=(const tmpl_stack<TCOPY,CONT_COPY>& s);

private:
	CONT<T> _copyable_stack;
};

// template <typename ELEM, typename = std::allocator<ELEM> > - определьние последовательных контейнеров STL
// теперь аргумент контейнера можно передавать в виде std::deque, а не std::deque<T>

// Определение методов класса с шаблонным параметром
// (Имена неиспользуемых параметров можно опустить)
template <typename T, template <typename, typename> class CONT>
tmpl_stack<T,CONT>::tmpl_stack(){}

template <typename T, template <typename, typename> class CONT>
void tmpl_stack<T,CONT>::push(const T &p){
	_copyable_stack.push_back(p);
}

template <typename T, template <typename, typename> class CONT>
void tmpl_stack<T,CONT>::pop(){
	_copyable_stack.pop_back();
}

template <typename T, template <typename, typename> class CONT>
const T& tmpl_stack<T,CONT>::top() const {
	return _copyable_stack.back();
}

template <typename T, template <typename, typename> class CONT>
bool tmpl_stack<T,CONT>::empty() const{
	return _copyable_stack.empty();
}


// Самый сложный метод - шаблонный метод в шаблонном классе, 
// содержащем сложный шаблонный параметр
template <typename T, template <typename, typename> class CONT>
template <typename TCOPY, template <typename, typename> class CONT_COPY>
tmpl_stack<T, CONT>& tmpl_stack<T, CONT>::operator=( const tmpl_stack<TCOPY, CONT_COPY>& s ) {

	tmpl_stack<TCOPY, CONT_COPY>  tmp(s);
	_copyable_stack.clear();

	// Копируем почленно с приведением типа (чтобы проверялось в момент компиляции)
	while( !tmp.empty() ){
		_copyable_stack.push_back( static_cast<T>( tmp.top() ) );
		tmp.pop();
	}
	return *this;
}

// Для шаблонов функций шаблонные параметры не допускаются!

// ------------------- Инициализация нулем -------------------
// Форма T() обеспечивает инициализацию по умолчанию для классов
// и инициализацию нулем для базовых типов
template <typename T>
class init_zero{
public:
	// Инициализация шаблонного параметра нулем
	init_zero() : _x() {}
private:
	T _x;
};

// ------------------- Стоковые литералы в качестве аргументов -------------------
// Преобразование массива в указатель присходит только в случае, если имя массва передается по значению

// 1. Версия с передачей по ссылке
template <typename T>
inline const T& str_max_ref( const T& a,const T& b ){
	return (a > b) ? a : b;
}

// 1. Версия с передачей по значению
template <typename T>
inline const T str_max_val( const T a,const T b ){
	return (a > b) ? a : b;
}

// Демонстрация преобразований
template <typename T>
inline void show_ref( const T& a){
	// Выведем значение параметра
	cout << "str_max_ref() param type is: " << typeid(a).name() << endl;
}

template <typename T>
inline void show_val( const T a ){
	// Выведем значение параметра (тип преобразован)
	cout << "str_max_val() param type is: " << typeid(a).name() << endl;
}
