#pragma once
#include <vector>
#include <cmath>
#include <cstddef>
// В классе рассмотрены приемы перегрузки операторов
// Перегрузка операторов применяется как для работы с конкретными типами, 
// так и при проектировании обобщенных и абстрактных интерфейсов: -> [] ()

/* Можно перегружать
+	-	*	/	=
%	^	&|	~		
==	<	>	!=	<=	>=
+=	-=	*=	/=
%=	^=	&=	|=
<<	>>	>>=	<<=
!	&&	||	
++	--
->	->*	[]	() ,
new	new[]	delete	delete[]
operator {type}()
*/

/*Нельзя перегружать
::	.	.*	?:
*/

// Нельзя использовать параметры по умолчанию, 
// т.к. иначе теоретически можно сделать бинарную ф-цию унарной

// Проектируя класс, следует ответить на следующие вопросы
// Как следует создавать и удалять объекты?
// Отличается ли инициализация от присвоения?
// Что означает передача по значению?
// Допустимые значения?
// Допустимые преобразования?
// Место в иерархии
// Какие стандартные операторы?
// Кто должен иметь доступ? Друзья?
// Насколько новый тип общий?


/** @brief 
Класс complex_t реализует арифметику комплексного числа
и демонстрирует принципы перегрузки операций в C++
*/
class complex_t
{
public:
	// Конструктор по умолчанию, он же просто конструктор
	complex_t(double r = 0., double i = 0.);

	// Исходите из того, что для большинства функций разумных значений по умолчанию нет
	// Существует неоднозначность при вызове целочисленных параметров и указателей (передача 0 - int)

	// Конструктор для неявного преобразования из int
	complex_t(int r);

	// --- Вспомогательные функции --- 
	inline double real() const;
	inline double imag() const;

	// --- Перегруженные операторы --- 
	// Операторы, которые лучше перегружать методом
	// должны возвращать ссылку на себя, т.к. изменяют свое состояние
	// для возможности записи в цепочку a += b += c ...
	// и предотвращения случайного образования неименованных объектов
	complex_t& operator+=( const complex_t& a );
	complex_t& operator-=( const complex_t& a );
	complex_t& operator*=( const complex_t& a );
	complex_t& operator/=( const complex_t& a );

	// Эти операторы константные, т.к. не изменяют состояние объекта
	bool operator==( const complex_t& a ) const;
	bool operator!=( const complex_t& a ) const;

	// Инкремент и декремент
	// префиксный - возвращает ссылку!
	complex_t& operator++();
	complex_t& operator--();
	// постфиксный - возвращает на стек значение, что хуже
	// принимает фиктивный параметр int
	complex_t operator++(int);
	complex_t operator--(int);

	// Операторы, которые можно перегружать, но в данном случае не нужно
	// operator=() - достаточно сгенерированного по умолчанию
	// ! то же для конструктора копирования
	// обратите внимание, что параметр конструктора копирования 
	// не может передаваться по значению - это столкнет его в бесконечную рекурсию

	// Операторы, которые обязательно перегружать методом класса - 
	// операторы преобразования типа - (!) не имеют возвращаемого значения,...
	operator int();
	operator double();
	// ... а также это следующие операторы:
	// =	в данном случае не нужен - возвращает ссылку на себя, т.к. модифицирует состояние
	//		для него необходима проверка на самоприсвоение!
	// ->	для умных указателей и реализации косвенного доступа (итераторы)
	// []	для проверки диапазона (!!! должен быть константный и неконстантный - Лаптев гл. 4 стр. 101)
	// ()	в функторах, операторах подстроки (substr) и для многомерных матриц

	// Можно преобразовывать в значение, означающее состояние объекта
	// Например в конструкции while( cin >> x ){}
	// cin >> x возвращает istream& и преобразуется в int

	// Операторы, перегруженные дружественными функциями
	// т.к. все параметры имеют тип complex_t&, значения типа int или double
	// могут быть неявно преобразованы - это дает большую гибкость и коммутативность операций
	// Можно также не объявлять их друзьями, а организовать интерфейс к закрытой части класса
	
	// Также важно то, что объявление дружественной функции м.б. единственным
	// т.е. не видным вне определенного класса, и даже может совпадать с определением
	// (см. пример с умноженеим и делением)
	friend const complex_t operator+( const complex_t& a, const complex_t& b );
	friend const complex_t operator-( const complex_t& a, const complex_t& b );

	// обьявление дружественной функции совпадает с определением
	friend const complex_t operator*( const complex_t& a, const complex_t& b ){
		complex_t tmp(a);
		return tmp *= b;
	}

	// обьявление дружественной функции совпадает с определением
	friend const complex_t operator/( const complex_t& a, const complex_t& b ){
		complex_t tmp(a);
		return tmp /= b;
	}

	// возвращаем по const значению, чтобы оно не было lvalue
	// чтобы нельзя было написать (a+b) = c


	// Радиус-вектор комплексного числа
	friend double r_vector(const complex_t& c){
		return sqrt( (c.re*c.re) + (c.im*c.im) );
	}
	
	// Бинарные операторы сложения можно определить как методом, так и функцией.
	// Удобнее это делать функцией, т.к. это обеспечивает коммутативность операции
	// и работает в случае, когда надо передать первый параметр, отличный от this ( << >> )
	// Их имеет смысл вывести в namespace 
	// ! При просмотре операторов нет преимущества у членов перед не-членами



private:
	double re, im;
};

// Мейерс 1 Правило 11
// Реализуйте глубокое копирование для классов с динамической памятью
// Если копирование-присвоение не нужно, запрещайте его

// Мейерс 1 Правило 12
// Списки инициализации эффективнее присвоения
// Как исключение, можно выносить сложную инициализацию в private функцию init()

// Мейерс 1 Правило 13
// во избежание ворнингов на некоторых компиляторах, 
// порядок инициализации должен совпадать с порядком объявления

// Мейерс 1 Правило 15
// operator=() должен возвращать ссылку на *this чтобы быть lvalue

// Мейерс 1 Правило 16
// если operator=() определен, нужно не забывать обновлять его после каждого добавления нового члена
// явно вызывайте операторы базового класса
// если возможно - испольуйте принцип подстановки


// Существуют методы, позволяющие избежать возвращения результата по значению
// (например если у него тяжелая инициализация)
// Например, храним уже инициализированные объекты в кэше
// (см. реализацию в complex.cpp)
complex_t& get_cache();
complex_t& operator%( const complex_t& a, const complex_t& b );



/** @brief 
Ссылка на Стандарт. Управление памятью.
Согласно Стандарта, различают 3 вида памяти - статическая, динамическая и автоматическая.
Основной единицей памяти является байт. Количество битов зависит от реализации.
Необходимо, чтобы в байт помещались базовые типы char, unsigned char.
Внимание - в Стандарте работа с динамической памятью описана во многих местах!
*/

/** @brief 
Ссылка на Стандарт. POD/non-POD типы.
По Стандарту POD-типы, это классы, 
- имеющие только автоматический конструктор
- не имеющие виртуальных функций
- если в классе есть другие классы, то они являются POD-типами
- если класс наследует от другого класса, то родитель является POD-типом
Для таких классов просто выделяется память

non-POD классы - это все остальные классы. Для них сначала выделяется память,
а потом вызывается конструктор
При невозможности выделить память конструктор не вызывается.

*/

// Мейерс 2 Правило 5 - переопределенные операторы приведения могут привести к неоднозначностям
// (например в паре с non-explicit конструктором)

// Мейерс 2 Правило 7 - перегрузка логических операторов && || ~ встречается крайне
// редко, поэтому малоожидаема и опасна
// К тому же, при перегрузке && || не будет "ленивых вычислений" - все выражения будут вычеслины
// При этом порядок вычисления не будет определен

/** @brief 
Класс complex_matrix реализует динамическую матрицу элементов complex_t
и предназначен исключительно для демонстрации работы с памятью 
и перегрузки необходимых в данном случае операций
*/
class complex_matrix{

protected:
	/** @brief Служебная функция очистки памяти без разрушения объекта */
	void clear();
public:
	// В конструкторах, где в качестве единственного параметра
	// передается количество элементов,
	// необходимо ограничивать с помощью explicit возможность 
	// неявной инициализации, например чтобы не было так: 
	// complex_matrix c_cet = 5;
	/** @brief Данный конструктор создает матрицу-строку */
	explicit complex_matrix(const size_t count_x);

	/** @brief Данный конструктор явно описывает размер матрицы */
	complex_matrix(const size_t count_x, const size_t count_y);

	/** @brief Конструктор копирования для данного класса должен реализовать
	так называемое "глубокое копирование" - deep copy, в противовес побитовому копированию 
	реализуемому по умолчанию  - т.н. "поверхностное копирование, shallow copy" */
	complex_matrix( const complex_matrix& c );

	/** @brief 
	Оператор присваивания также должен быть "глубоким".
	Кроме того, он должен возвращать старую память после присваивания */
	complex_matrix& operator=( const complex_matrix& c );

	// Существует более элегантный способ присваивания - 
	// с помощью операции swap (см. следующую реализацию в cpp)

	// Деструкторы не генерируют исключений.
    ~complex_matrix();

	// Для операторов индексирования необходимо предоставлять две версии - 
	// обычную и константную. Константная нужна для возвращения значения на стек
	// Обычная возвращает ссылку на значение и нужна, чтобы выражение с индексом
	// могло использоваться слева от знака = (было l-value)
	// Неконстантный оператор operator[] в данном случае не имеет смысла,
	// т.к. должен модифицировать указатель. Он приведен для примера.
	complex_t* operator[](unsigned index);
	const complex_t* operator[](unsigned index) const;

	// В реализации многомерных матриц для индексации лучше использовать оператор ()
	// вместо [], т.к. операция [] позволяет передачу только одного индекса
	// Возвращаемое по ссылке значение тоже должно быть константным!
	complex_t& operator()(unsigned index_x, unsigned index_y);
	const complex_t& operator()(unsigned index_x, unsigned index_y) const;

	// Друзей можно объявлять в закрытой или открытой части класса - не имеет значения.
	// Друзьями также можно объявлять отдельные функции класса...
	friend double complex_t::real() const;
	friend double complex_t::imag() const;

	// ...или весь класс (хотя в этом случае лучше применить открытый интерфейс
	// , композицию или наследование)
	friend class complex_t;

	// Класс-друг должен быть объявлен в охватывающей области видимости
	// Функция друг также должна быть объявлена в охватывающей области видимости,
	// ЛИБО иметь аргумент этого класса (пример с коммутативными операторами)

private:
	size_t _count_x;
	size_t _count_y;
	complex_t** _matrix;
};