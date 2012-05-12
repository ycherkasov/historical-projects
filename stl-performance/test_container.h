/** @file test_container.h
Здесь определен основной прототип шаблона тестового набора, 
подходящий под большинство наборов данных и контейнеров, и основные типы для работы с ним.
*/

#ifndef _PERF_TEST_TESTCONT_
#define _PERF_TEST_TESTCONT_
#include <boost/timer.hpp>
#include "random_data_generator.h"
#include "clock_count.h"
#include "stl.h"

/**@brief 
Список поддерживаемых контейнеров (может изменяться пользователем!) */
enum containers_t{
	cont_vector,
	cont_list,
	cont_deque,
	cont_set,
	cont_multiset,
	cont_map,
	cont_multimap,
	cont_hash_set,
	cont_hash_map,
	containers_type_count
};

// ------------------  -------------------
/**@brief База для ассоциативных контейнеров set */
template < typename Element, template 
<typename Element, typename = std::less<Element>,  typename = std::allocator<Element> >
	class Container_t = std::set >
class associative_base_set{
public:
	typedef Container_t<Element> Container;
};

// ------------------  -------------------
/**@brief База для ассоциативных контейнеров map */
template < typename Key, typename Element, template 
<typename Key, typename Element, typename = std::less<Element>,  typename = std::allocator<Element> >
class Container_t = std::map >
class associative_base_map{
public:
	typedef Container_t<Key, Element> Container;
};

// ------------------  -------------------
/**@brief База для последовательных контейнеров  */
template < typename Element, template 
< typename Element, typename = std::allocator<Element> >
class Container_t = std::vector >
class serial_base{
public:
	typedef Container_t<Element> Container;
};

// ------------------- Классы реализаций -------------------
// Выполняется наследование простых классов, чтобы не наследовать от шаблонов 
// и проще определять новые специализации
class assiciative_set_impl	:	public associative_base_set<int>{};
class assiciative_multiset_impl	:	public associative_base_set<int,std::multiset>{};
class assiciative_map_impl	:	public associative_base_map<int, int >{};
class assiciative_multimap_impl	:	public associative_base_map<int, int, std::multimap >{};
class serial_vector_impl	:	public serial_base< int, std::vector >{};
class serial_list_impl		:	public serial_base< int, std::list >{};
class serial_deque_impl		:	public serial_base< int, std::deque >{};



// ------------------- Базовый класс общих операций -------------------
/**@brief 
Самый общий шаблон для тестового набора. Поддерживает основные операции.
*/
template < typename ContainerBase
, typename Element = int
, typename Counter = clock_count
, template < typename Element, typename = randint > class Generator = static_data_generator >
class container_test_base : public ContainerBase {
public:
	typedef typename ContainerBase::Container Container;

protected:
	container_test_base(unsigned count, bool sorted = false)
		// Генерация случайных данных
		: _generator( count, static_cast<unsigned>(time(0)) ){
	}

public:

	/** @brief Создание контейнера заданного размера из произвольных данных.
	*/
	double creation() {
		{
			Counter t;
			Container cont(_generator.begin(), _generator.end());
		}
		return Counter::get_last_count();
	}

	/** @brief Заполнение существующего контейнера заданным количеством 
		произвольных данных.
	*/
	double fill(){
		// создаем за пределами области видимости таймера, 
		// чтобы исключить накладные расходы
		Container cont;
		{
			Counter t;
			cont.assign( _generator.begin(), _generator.end() );
		}
		return Counter::get_last_count();
	}

	/** @brief Постепенное добавление данных в конец.
		@param count Количество добавляемых элементов.
	*/
	double push_data(size_t count){
		Container cont;
		{
			Counter t;
			for(unsigned i = 0 ; i < count ; i++ )
				cont.push_back( _generator.random_data() );
		}
		return Counter::get_last_count();
	}

	/** @brief Добавление целого интервала данных.
		@param Количество добавляемых элементов.
	*/
	double add_range(size_t count){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			std::copy( _generator.begin(), _generator.range(count), back_inserter(cont) );
		}
		return Counter::get_last_count();
	}

	/** @brief Итерация по контейнеру.
	*/
	double iterate(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			Container::iterator it = cont.begin();
			while( it != cont.end() ){
				++it;
			}
		}
		return Counter::get_last_count();
	}


	/** @brief Итерация по контейнеру с удалением.
	*/
	double iterate_erase(){
		unsigned int counter = 0;
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			Container::iterator it = cont.begin();
			while( !cont.empty() ){
				Container::iterator ers = cont.begin();
				cont.erase(ers);
				++counter;
			}
		}
		return Counter::get_last_count();
	}


	/** @brief Удаление каждого n-го элемента во время итерации.
	*/
	double erase_some(size_t every){
		Container cont( _generator.begin(), _generator.end() );
		{
			unsigned counter = 0;
			Counter t;
			Container::iterator ers = cont.begin();
			while( ers != cont.end() ){
				// refactor this!!!
				if( (counter%every) == 0 )
					ers = cont.erase(ers);
				else 
					++ers;
				++counter;
			}
		}
		return Counter::get_last_count();
	}



	/** @brief Удаление интервала с начала конейнера.
		@param Количество удаляемых элементов.
	*/
	double erase_begin(size_t range){
		Container cont( _generator.begin(), _generator.end() );
		

		{
			Counter t;
			cont.erase( cont.begin(), cont.begin() + range );
		}
		return Counter::get_last_count();
	}

	/** @brief Удаление интервала с конца конейнера.
		@param Количество удаляемых элементов.
	*/
	double erase_end(size_t range){
		Container cont( _generator.begin(), _generator.end() );


		{
			Counter t;
			cont.erase( cont.end() - range, cont.end() );
		}
		return Counter::get_last_count();
	}

	/** @brief Очистка конейнера.
	*/
	double erase(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			cont.clear();
		}
		return Counter::get_last_count();
	}
	
	/** @brief	Поиск элемента по контейнеру.
				Элемент выбирается случайно из сгенерированной последовательности.
	*/
	double find(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			std::find(cont.begin(), cont.end(), _generator.random_data());
		}
		return Counter::get_last_count();
	}

	/** @brief	Поиск подпоследовательности по контейнеру.
				Подпоследовательность выбирается случайно из сгенерированной последовательности.
	*/
	double search(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			// todo : рандомность диапазона
			Container::const_iterator res = 
				std::search( cont.begin(), cont.end(), cont.begin(), cont.end() );
		}
		return Counter::get_last_count();
	}

	/** @brief Копирование контейнера.
	*/
	double copy(){
		Container cont1( _generator.begin(), _generator.end() );
		Container cont2( _generator.begin(), _generator.end() );
		{
			Counter t;
			std::copy( cont1.begin(),cont1.end(),cont2.begin() );
		}
		return Counter::get_last_count();
	}

	/** @brief Модификация элементов контейнера.
	*/
	double transform(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			std::transform( cont.begin(), cont.end(), cont.begin()
				,  std::bind2nd(multiplies<Element>(), 2) );
		}
		return Counter::get_last_count();
	}

	/** @brief Реверс элементов контейнера.
	*/
	double reverse(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			std::reverse( cont.begin(), cont.end() );
		}
		return Counter::get_last_count();
	}

	/** @brief Перемешивание элементов.
	*/
	double random_shuffle(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			std::random_shuffle( cont.begin(), cont.end() );
		}
		return Counter::get_last_count();
	}

	/** @brief Неустойчивая сортировка.
	*/
	double sort(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			std::sort( cont.begin(), cont.end() );	
		}
		return Counter::get_last_count();
	}

	/** @brief Устойчивая сортировка.
		@return Время выполнения алгоритма.
	*/
	double stable_sort(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			std::stable_sort( cont.begin(), cont.end() );
		}
		return Counter::get_last_count();
	}

	/** @brief Поиск в отсортированной последовательности.
	*/
	double find_sorted(){
		Container cont( _generator.begin(), _generator.end() );
		std::sort( cont.begin(), cont.end() );
		{
			Counter t;
			std::lower_bound( cont.begin(), cont.end(), _generator.random_data() );
		}
		return Counter::get_last_count();
	}

	/** @brief Слияние отсортированых последовательностей.
	*/
	double merge_sorted(){
		Container cont1( _generator.begin(), _generator.end() );
		Container cont2( _generator.begin(), _generator.end() );
		std::sort( cont1.begin(), cont1.end() );
		std::sort( cont2.begin(), cont2.end() );
		Container cont_dest;
		{
			Counter t;
			std::merge( cont1.begin(), cont1.end(), cont2.begin(), cont2.end(), back_inserter(cont_dest) );
		}
		return Counter::get_last_count();
	}

	/** @brief Логическая сумма (объединение) множеств.
	*/
	double set_union(){
		Container cont1( _generator.begin(), _generator.end() );
		Container cont2( _generator.begin(), _generator.end() );
		Container cont_out;
		std::sort( cont1.begin(), cont1.end() );
		std::sort( cont2.begin(), cont2.end() );
		{
			Counter t;
			std::set_union( cont1.begin(), cont1.end()
				, cont2.begin(), cont2.end(), back_inserter(cont_out) );
		}
		return Counter::get_last_count();
	}

	/** @brief Логическое умножение (пересечение) множеств.
	*/
	double set_intersection(){
		Container cont1( _generator.begin(), _generator.end() );
		Container cont2( _generator.begin(), _generator.end() );
		Container cont_out;
		std::sort( cont1.begin(), cont1.end() );
		std::sort( cont2.begin(), cont2.end() );
		{
			Counter t;
			std::set_intersection( cont1.begin(), cont1.end()
				, cont2.begin(), cont2.end(), back_inserter(cont_out) );
		}
		return Counter::get_last_count();
	}
protected:
	size_t _sz;
	bool _sorted;
	Generator<Element> _generator;
};



#endif //_PERF_TEST_TESTCONT_