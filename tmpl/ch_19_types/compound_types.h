#pragma once

// Составные типы - это массивы, указетли и ссылки

// Базовый шаблон для всех типов - указатель
template <typename T>
struct is_pointer{
	static bool result(){return false;}
};

// Базовый шаблон для всех типов - ссылка
template <typename T>
struct is_reference{
	static bool result(){return false;}
};

// Базовый шаблон для всех типов - массив
template <typename T>
struct is_array{
	static bool result(){return false;}
};

// Базовый шаблон для всех типов - указатель на функцию
template <typename T>
struct is_function_pointer{
	static bool result(){return false;}
};

// Базовый шаблон для всех типов - указатель на метод
template <typename T>
struct is_method_pointer{
	static bool result(){return false;}
};

//////////////////////////////////////////////////////////////////////////

// Специализация - указатель
template <typename T>
struct is_pointer<T*>{
	static bool result(){return true;}
};

// Специализация - ссылка
template <typename T>
struct is_reference<T&>{
	static bool result(){return true;}
};

// Специализации - массив
template <typename T, int N>
struct is_array<T[N]>{
	static bool result(){return true;}
};

// Форма для массива без указания количества элементов
template <typename T>
struct is_array<T[]>{
	static bool result(){return true;}
};

// Базовый шаблон для всех типов - указатель на функцию
// без параметров
template <typename T>
struct is_function_pointer<T()>{
	static bool result(){return true;}
};

// с 1 параметром
template <typename T, typename P>
struct is_function_pointer<T(P)>{
	static bool result(){return true;}
};

// с 2 и более параметров
template <typename T, typename P1>
struct is_function_pointer<T(P1, ...)>{
	static bool result(){return true;}
};

// Базовый шаблон для всех типов - указатель на метод
template <typename T, typename C>
struct is_method_pointer<T C::*>{
	static bool result(){return true;}
};


//////////////////////////////////////////////////////////////////////////
// TODO: is_function, other methods: SFINAE, * to &
