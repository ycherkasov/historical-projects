/** @brief 
Глава 13 Страуструпа
Простые шаблоны. Определение, инстанцирование, параметры, эквивалентность, проверка типов
Шаблоны функций.
Шаблоны по выбору алгоритма.
Специализация. Наследование и шаблоны. Преобразования шаблонов.
*/

#include <vector>
#include <string>
#include <iostream>

using namespace std;


// ----------------------------------
// Абстрактный класс шаблона - 
// может использоваться как обычный класс 
// для создания общего интерфейса
// Шаблон может наследовать также от обычного класса.
template<typename Symbols>
class compare_str{
public:
	virtual void compare(const basic_string<Symbols>& str) = 0;
};

// ----------------------------------
// Производные шаблонные классы
// с виртуальными методами
template<typename Symbols>
class lexigraphical : public compare_str<Symbols>{
public:
	virtual void compare(const basic_string<Symbols>& str);
};

template<typename Symbols>
class no_case : public compare_str<Symbols>{
public:
	virtual void compare(const basic_string<Symbols>& str);
};

// --------------- реализация методов --------------- 
template<typename Symbols>
void lexigraphical<Symbols>::compare(const basic_string<Symbols>& str){
	std::cout << "lexigraphical<Symbols>::compare()" << '\n';
}

template< typename Symbols >
void no_case<Symbols>::compare(const basic_string<Symbols> &str){
	std::cout << "no_case<Symbols>::compare()" << '\n';
}
// ----------------------------------


// ----------------------------------
// Шаблонный класс, в который передается 2 параметра
// тип символов и тип сравнения
// Последний имеет значение по умолчанию
template<typename Symbols, typename Comparer = lexigraphical>
class container{
	basic_string<Symbols> _str;
	Comparer _c;
public:
	container(basic_string<Symbols>& str);
	void compare(basic_string<Symbols>& str);
};

template<typename Symbols, typename Comparer>
container<Symbols, Comparer>::container(basic_string<Symbols>& str) :
	_str(str)
{}

// Используем переданный тип класса для сравнения строк
template<typename Symbols, typename Comparer>
void container<Symbols, Comparer>::compare(basic_string<Symbols>& str){
	_c.compare(_str);
}

// Пример - частичной специализации - 
// Вектор для хранения только указателей
// наследует интерфейс от vector<void*>
// Чтобы определить тип элементов как указатель
// явно укажем параметр шаблона vector<T*>
// Специализация - это "перегрузка" для классов

// Наследование частичной специализации от полной, чтобы сократить дублирование кода
// - это паттерн шаблонного программирования
template< typename T >
class vector< T* > : public vector< void* >{
public:
	typedef vector<void*>	base;

	vector():base(){}
	explicit vector(size_t i) : base(i){}

	T*& elem(size_t i){ return static_cast<T*&>(base::elem(i)); }
	T& operator[](size_t i){ return static_cast<T*&>(base::operator [](i) ); }
};