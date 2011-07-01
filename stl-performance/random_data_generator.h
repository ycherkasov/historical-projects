#ifndef _PERF_TEST_RND_GENERATOR_
#define _PERF_TEST_RND_GENERATOR_
#include "stl.h"
#include "command_line_params.h"
#include "random_data_generator.h"
#include "randint.h"

/** @file random_data_generator.h
Здесь реализован ряд генераторов случайных данных различных типов.
*/

/**@brief 
Функция-синглтон для генераторов (чтобы пользоваться одним генератором все 
время работы приложения)
*/
template <typename T, template 
	<typename TB, typename RInit = randint > class Base>
Base<T>& get_base_generator(){
	static Base<T> _base( get_params()._size );
	return _base;
}

/**@brief 
Класс, предназначенный для генерации набора 
случайных данных произвольного типа */
template<typename T, typename Generator = randint >
class random_data_generator{
public:
	explicit random_data_generator(unsigned count, int seed = 0) 
		: _count(count){
		_cont = new T[count];
		std::generate(_cont, _cont + count, Generator(seed) );
	}
	~random_data_generator(){
		delete[] _cont;
	}
	
	T* random(){
		Generator g( static_cast<long>(time(0)) );
		return _cont + (g()%_count);
	}
	T random_data(){
		Generator g( static_cast<long>(time(0)) );
		return *(_cont + (g()%_count));
	}
	T* begin(){return _cont;}
	T* range(size_t count){return (_cont + _count);}
	T* end(){return (_cont + _count);}
private:
	unsigned _count;
	T* _cont;
};


/**@brief 
Класс, предназначенный для генерации набора случайных данных 
типа std::pair (для специализации map)*/
template<typename T, typename Generator = randint >
class random_pair_generator{
public:
	explicit random_pair_generator(unsigned count, int seed = 0) 
		: _count(count){
			T* cont = new T[count];
			for(unsigned i = 0 ; i < count ; i++){
				_cont[i] = cont[i];
			}
			delete cont;
	}
	~random_pair_generator(){}

	typename std::map<const T,T>::iterator random() const {	
		Generator g(3242);
		return _cont.find( (g()%_count) );
	}
	T random_data(){
		Generator g(3242);
		return (*_cont.find( (g()%_count) )).second;
	}
	typename std::map<const T,T>::iterator begin(){return _cont.begin();}
	typename std::map<const T,T>::iterator range(size_t count){
		return _cont.find( static_cast<T>(count) );/* [];*/
	}
	typename std::map<const T,T>::iterator end(){return _cont.end();}
private:
	unsigned _count;
	std::map<const T,T> _cont;
};

/**@brief 
Класс-обертка для генератора случайных данных 
(нужен, чтобы не перегенерировать данные каждый раз) */
template<typename T, typename Generator = randint  >
class static_data_generator{
public:
	static_data_generator(unsigned count, int seed = 0){}
	T* random(){	
		return get_base_generator<T, random_data_generator >().random();
	}
	T random_data(){
		random_data_generator<T>& g = 
			get_base_generator<T, random_data_generator >();
		return g.random_data();
	}
	T* begin(){
		return get_base_generator<T, random_data_generator>().begin();
	}

	T* range(size_t count){
		return get_base_generator<T, random_data_generator>().range(count);
	}

	T* end(){
		return get_base_generator<T, random_data_generator>().end();
	}
};

/**@brief 
Класс-обертка для генератора случайных данных типа std::pair
(нужен, чтобы не перегенерировать данные каждый раз) */
template<typename T, typename Generator = randint  >
class static_pair_generator{
public:
	static_pair_generator(unsigned count, int seed = 0){}
	T* random(){	
		return get_base_generator<T, random_pair_generator>().random();
	}

	T random_data(){
		return get_base_generator<T, random_pair_generator>().random_data();
	}
	typename std::map<const T,T>::iterator  begin(){
		return get_base_generator<T, random_pair_generator>().begin();
	}
	
	typename std::map<const T,T>::iterator  range(size_t count){
		return get_base_generator<T, random_pair_generator>().range(count);
	}

	typename std::map<const T,T>::iterator  end(){ 
		return get_base_generator<T, random_pair_generator>().end();
	}
};

#endif //_PERF_TEST_RND_GENERATOR_