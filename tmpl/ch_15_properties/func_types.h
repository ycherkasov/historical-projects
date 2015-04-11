#pragma once
// Глава 15.2 Джосаттиса
// Функции типа

// Функции типа - это шаблоны, принимающие типы и возвращающеие тип или константу
// Например, размер типа
template <typename T>
size_t type_size(const T& t) {
	return sizeof(T);
}

// Определение, является ли данный тип классом (основан на SFINAE)
template <typename T>
class is_class{
private:
	typedef char One;
	typedef struct{char a[2];} Two;
	template <typename C> static One test(int C::*);
	template <typename C> static Two test(...);
public:
	enum{ Yes = sizeof(is_class<T>::test<T>(0)) == 1 };
	enum{ No = !Yes };
};

// Типы для тестирования
class MyClass{
};

struct MyStruct{
};

union MyUnion{
};

void MyFunc(){

}

enum E {e1} e;

