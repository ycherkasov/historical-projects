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
#include <algorithm>

using namespace std;


// ----------------------------------
// Абстрактный класс шаблона -
// может использоваться как обычный класс
// для создания общего интерфейса
// Шаблон может наследовать также от обычного класса.
template<typename Symbols>
class compare_str{
public:
    virtual bool compare(const basic_string<Symbols>& str1, const basic_string<Symbols>& str2) = 0;
};

// ----------------------------------
// Производные шаблонные классы
// с виртуальными методами
template<typename Symbols>
class lexigraphical : public compare_str<Symbols>{
public:
    virtual bool compare(const basic_string<Symbols>& str1, const basic_string<Symbols>& str2);
};

template<typename Symbols>
class no_case : public compare_str<Symbols>{
public:
    virtual bool compare(const basic_string<Symbols>& str1, const basic_string<Symbols>& str2);
};

// --------------- реализация методов ---------------
template<typename Symbols>
bool lexigraphical<Symbols>::compare(const basic_string<Symbols>& str1, const basic_string<Symbols>& str2){
    return str1 > str2;
}

template< typename Symbols >
bool no_case<Symbols>::compare(const basic_string<Symbols> &str1, const basic_string<Symbols>& str2){
    std::string nocase1, nocase2;
    std::transform(str1.begin(), str1.end(), std::back_inserter(nocase1), ::tolower);
    std::transform(str2.begin(), str2.end(), std::back_inserter(nocase2), ::tolower);
	return nocase1 > nocase2;
}
// ----------------------------------


// ----------------------------------
// Шаблонный класс, в который передается 2 параметра
// тип символов и тип сравнения
// Последний имеет значение по умолчанию
template<typename Symbols, typename Comparer = lexigraphical<Symbols> >
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
    _c.compare(str, _str);
}

// Пример - частичной специализации -
// Вектор для хранения только указателей
// наследует интерфейс от vector<void*>
// Чтобы определить тип элементов как указатель
// явно укажем параметр шаблона vector<T*>
// Специализация - это "перегрузка" для шаблонных классов

// Наследование частичной специализации от полной, чтобы сократить дублирование кода
// - это паттерн шаблонного программирования
#if 0
template< typename T >
class std::vector< T*, allocator<T*> > : public std::vector< void*, allocator<void*> >{
public:
    typedef vector<void*, allocator<void*>> base;

    vector< T*, allocator<T*> >() : vector< void*, allocator<void*>>(){}
    explicit vector<T, allocator<T*> >(size_t i) : base(i){}

    T*& elem(size_t i){ return static_cast<T*&>(base::elem(i)); }
    T& operator[](size_t i){ return static_cast<T*&>(base::operator [](i) ); }
};
#endif
