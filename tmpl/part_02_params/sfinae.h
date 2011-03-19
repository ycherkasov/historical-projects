#pragma once

// Принцип SFINAE (Substitution Failure is not an error)
// (Неверная подстановка не является ошибкой)

// Определим 2 типа данных
// sizeof первого = 1, второго = 2
typedef char RT1;	
typedef struct{char a[2];}  RT2;

struct X{
	char a[1];
};

class test_X{
public:
	X x;
};

// Перегрузим 2 шаблона, возвращающих эти типы в зависимости от параметров
// Если в типе есть член X, то вернет первый, иначе - второй
template <typename T> RT1 test_sfinae_x(typename T::X const&){
	return 0;
}

template <typename T> RT2 test_sfinae_x(...){ 
	RT2 rt = {};
	rt.a[0] = 'a';
	rt.a[1] = 0;
	return rt;
}

// Определим макрос, проверяющий размер типа, возвращаемого шаблоном
// Если был передан первый тип, то условие = true и был передан тип,
// содержащий член X
#define type_has_member_type_X(T) ( ( sizeof(test_sfinae_x<T>() ) ) == 1 )

// Важно - SFINAE защищает от неверных типов, но не от неверных вычислений

// Еще один пример со SFINAE
template <int N> int get_int(){ return N; }
template <int* P> int get_int(){ return *P; }

int test_sfinae_int(){
	// Раскомментировать для ошибки
	//return get_int<1>();
	// Одна из приведенных функций дает ошибку до связывания с параметром шаблона
	return 0;
}