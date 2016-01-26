/** special.h
Здесь определены специализации тестовых наборов для различных контейнеров
*/

#ifndef _PERF_TEST_SPECIAL_
#define _PERF_TEST_SPECIAL_

#include "test_container.h"

// -------------------  -------------------
/**@brief 
Общая специализация "для всех" */
template < typename ContainerBase
, typename Element = int
, typename Counter = clock_count
, template < typename Element, typename = randint > class Generator = static_data_generator >
class container_test : public container_test_base< ContainerBase, Element, Counter, Generator > {
public:
	container_test(unsigned count, bool sorted = false) : container_test_base(count, sorted){}
};

// -------------------  -------------------
/**@brief 
Специализация для вектора */
template < typename Element, typename Counter, typename Generator >
class container_test<serial_vector_impl, Element, Counter, Generator> 
	: public container_test_base< serial_vector_impl, Element, Counter, Generator > {
public:
	typedef std::vector<Element> Container;

	container_test(unsigned count, bool sorted = false) : container_test_base(count, sorted){}

	/** @brief Итерация по вектору с удалением 
	приводит к критическому падению быстродействия.
	*/
	double iterate_erase(){
		return -1.0;
	}
};

// -------------------  -------------------
/**@brief 
Специализация для списка */
template < typename Element, typename Counter, typename Generator >
class container_test<serial_list_impl, Element, Counter, Generator> 
	: public container_test_base< serial_list_impl, Element, Counter, Generator > {
public:
	typedef serial_list_impl::Container Container;

	container_test(unsigned count, bool sorted = false) : container_test_base(count, sorted){}

	/** @brief Список не поддерживает итераторы случайного доступа.
	Для выделения диапазона двунаправленных итератороы используйте std::advance()
	*/
	double erase_begin(Container::size_type range){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			Container::iterator it = cont.begin();
			std::advance(it, range);
			cont.erase( cont.begin(),  it );
		}
		return Counter::get_last_count();
	}

	/** @brief Список не поддерживает итераторы случайного доступа.
	Для выделения диапазона двунаправленных итератороы используйте std::advance()
	*/
	double erase_end(Container::size_type range){
		Container cont( _generator.begin(), _generator.end() );

		{
			Counter t;
			Container::iterator it = cont.begin();
			Container::size_type sz = cont.size();
			std::advance(it, sz - range);
			cont.erase( it, cont.end() );
		}
		return Counter::get_last_count();
	}


	/** @brief Список не поддерживает итераторы случайного доступа.
	Алгоритм random_shuffle() не имеет смысла в списке.
	*/
	inline double random_shuffle(){
		return -1.0;
	}

	/** @brief Список имеет собственный метод для сортировки.
	Это обусловлено тем, что std::sort() требует поддержки
	итераторов случайного доступа.
	*/
	double sort(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			cont.sort();
		}
		return Counter::get_last_count();
	}

	/** @brief Сортировка списка всегда устойчива,
	нет необходимости в отдельном алгоритме.
	*/
	inline double stable_sort(){
		return -1.0;
	}

	/** @brief Список не поддерживает итераторы случайного доступа.
	Сложность поиска линейна.
	*/
	inline double find_sorted(){
		Container cont( _generator.begin(), _generator.end() );
		cont.sort();
		{
			Counter t;
			std::lower_bound( cont.begin(), cont.end(), _generator.random_data() );
		}
		return Counter::get_last_count();
	}

	/** @brief Список имеет собственный метод для слияния.
	Это обусловлено тем, что std::sort() требует поддержки
	итераторов случайного доступа.
	*/
	double merge_sorted(){
		std::list<Element> cont1( _generator.begin(), _generator.end() );
		std::list<Element> cont2( _generator.begin(), _generator.end() );
		cont1.sort();
		cont2.sort();
		std::list<Element> cont_dest;
		{
			Counter t;
			cont1.merge(cont2);
		}
		return Counter::get_last_count();
	}

	/** @brief Список не поддерживает итераторы случайного доступа.
	*/
	double set_union(){
		Container cont1( _generator.begin(), _generator.end() );
		Container cont2( _generator.begin(), _generator.end() );
		Container cont_out;
		cont1.sort();
		cont2.sort();
		{
			Counter t;
			std::set_union( cont1.begin(), cont1.end()
				, cont2.begin(), cont2.end(), back_inserter(cont_out) );
		}
		return Counter::get_last_count();
	}

	/** @brief Список не поддерживает итераторы случайного доступа.
	*/
	double set_intersection(){
		Container cont1( _generator.begin(), _generator.end() );
		Container cont2( _generator.begin(), _generator.end() );
		Container cont_out;
		cont1.sort();
		cont2.sort();
		{
			Counter t;
			std::set_intersection( cont1.begin(), cont1.end()
				, cont2.begin(), cont2.end(), back_inserter(cont_out) );
		}
		return Counter::get_last_count();

	}

};

// -------------------  -------------------
/**@brief 
Специализация для множества */
template < typename Element, typename Counter, typename Generator >
class container_test< assiciative_set_impl, Element, Counter, Generator > 
	: public container_test_base< assiciative_set_impl, Element, Counter, Generator > {
public:
	typedef assiciative_set_impl::Container Container;

	container_test(unsigned count, bool sorted = false) : container_test_base(count, sorted){}

	/** @brief Для инициализации множества используются 
	только базовые операции присваивания.
	*/
	double fill(){
		Container cont;
		{
			Counter t;
			Container c( _generator.begin(), _generator.end() );
			cont = c;
		}
		return Counter::get_last_count();
	}

	/** @brief "Добавление данных в конец" не имеет смысла в множестве.
	Вместо него используется вставка - insert()
	*/
	double push_data(Container::size_type count){
		Container cont;
		{
			Counter t;
			for(unsigned i = 0 ; i < count ; i++ )
				cont.insert( _generator.random_data() );
		}
		return Counter::get_last_count();
	}

	/** @brief "Добавление данных в конец" не имеет смысла в множестве.
	Вместо него используется вставка - insert()
	*/
	double add_range(Container::size_type count){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			cont.insert( _generator.begin(), _generator.range(count) );
		}
		return Counter::get_last_count();
	}


	/** @brief Множество не поддерживает итераторы случайного доступа.
	@param Количество удаляемых элементов.
	*/
	double erase_begin(Container::size_type range){
		Container cont( _generator.begin(), _generator.end() );

		{
			Counter t;
			std::set<Element>::iterator it = cont.begin();
			std::advance(it, range);
			cont.erase( cont.begin(),  it );
		}
		return Counter::get_last_count();
	}

	/** @brief Множество не поддерживает итераторы случайного доступа.
	@param Количество удаляемых элементов.
	*/
	double erase_end(Container::size_type range){
		Container cont( _generator.begin(), _generator.end() );

		{
			Counter t;
			std::set<Element>::iterator it = cont.begin();
			std::set<Element>::size_type sz = cont.size();
			std::advance(it, sz - range);
			cont.erase( it, cont.end() );
		}
		return Counter::get_last_count();
	}

	/** @brief Множество не поддерживает итераторы случайного доступа.
	Метод не имеет смысла в множестве
	*/
	inline double random_shuffle(){
		return -1.0;
	}


	/** @brief Метод не имеет смысла в множестве
	*/
	inline double merge_sorted(){
		return -1.;
	}

	/** @brief Сортировка не требуется в множестве
	*/
	inline double sort(){
		return -1.;
	}

	/** @brief Сортировка не требуется в множестве
	*/
	double find_sorted(){
		return find();
	}

	/** @brief Сортировка не требуется в множестве
	*/
	inline double stable_sort(){
		return -1.0;
	}

	/** @brief Множество не поддерживает итераторы случайного доступа.
	*/
	double set_union(){
		Container cont1( _generator.begin(), _generator.end() );
		Container cont2( _generator.begin(), _generator.end() );
		Container cont_out;
		{
			Counter t;
			std::set_union( cont1.begin(), cont1.end()
				, cont2.begin(), cont2.end(), std::inserter(cont_out, cont_out.begin()) );
		}
		return Counter::get_last_count();
	}

	/** @brief Множество не поддерживает итераторы случайного доступа.
	*/
	double set_intersection(){
		Container cont1( _generator.begin(), _generator.end() );
		Container cont2( _generator.begin(), _generator.end() );
		Container cont_out;
		{
			Counter t;
			std::set_intersection( cont1.begin(), cont1.end()
				, cont2.begin(), cont2.end(), std::inserter(cont_out, cont_out.begin()) );
		}
		return Counter::get_last_count();

	}
};

// -------------------  -------------------
/**@brief 
Специализация для мультимножества */
template < typename Element, typename Counter, typename Generator >
class container_test< assiciative_multiset_impl, Element, Counter, Generator > 
	: public container_test< assiciative_set_impl, Element, Counter, Generator > {
public:
	container_test(unsigned count, bool sorted = false)
		: container_test< assiciative_set_impl, Element, Counter, Generator >(count, sorted){}
	
};

// ------------------- -------------------
/**@brief 
Специализация для мапа  */
template < typename Element, typename Counter, typename Generator >
class container_test< assiciative_map_impl, Element, Counter, Generator > 
	: public container_test_base< assiciative_map_impl, Element, Counter, Generator > {
public:
	container_test(unsigned count, bool sorted = false) : container_test_base(count, sorted){}

	typedef assiciative_map_impl::Container Container;

	/** @brief Для инициализации мапа используются 
	только базовые операции присваивания.
	*/
	double fill(){
		Container cont1( _generator.begin(), _generator.end() );
		Container cont;
		{
			Counter t;
			
			cont.swap(cont1);
		}
		return Counter::get_last_count();
	}

	/** @brief "Добавление данных в конец" не имеет смысла в мапе.
	Вместо него используется вставка - insert()
	*/
	double push_data(Container::size_type count){
		Container cont;
		{
			Counter t;
			for(unsigned i = 0 ; i < count ; i++ )
				cont.insert( std::make_pair(i, _generator.random_data() ) );
		}
		return Counter::get_last_count();
	}

	/** @brief "Добавление данных в конец" не имеет смысла в множестве.
	Вместо него используется вставка - insert()
	*/
	double add_range(Container::size_type count){
		Container cont( _generator.begin(), _generator.end() );
		Container cont2( _generator.begin(), _generator.range(count) );
		{
			Counter t;
			cont.insert( cont2.begin(), cont2.end() );
		}
		return Counter::get_last_count();
	}

	/** @brief Итерация по контейнеру с удалением.
	*/
	double iterate_erase(){
		Container cont( _generator.begin(), _generator.end() );
		Container::iterator it = cont.begin();
		{
			while(it != cont.end()){
				Container::iterator ers = it;		
				++it;
				cont.erase(ers);
			}
			Counter t;
		}
		return Counter::get_last_count();
	}


	/** @brief Удаление каждого n-го элемента во время итерации.
	*/
	double erase_some(Container::size_type every){
		Container cont( _generator.begin(), _generator.end() );
		Container::iterator it = cont.begin();
		int counter = 0;
		{
			while(counter){
				Container::iterator ers = it;		
				++counter; ++it;
				if( (counter%every) == 0 )
					cont.erase(ers);
			}
			Counter t;
		}
		return Counter::get_last_count();
	}

	/** @brief Удаление интервала с начала конейнера.
	@param Количество удаляемых элементов.
	*/
	double erase_begin(Container::size_type range){
		Container cont( _generator.begin(), _generator.end() );
		Container::iterator it = cont.begin();
		Container::size_type count = range;
		{
			while(count){
				Container::iterator ers = it;		
				--count; ++it;
				cont.erase(ers);
			}
			Counter t;
		}
		return Counter::get_last_count();
	}

	/** @brief Удаление интервала с конца конейнера.
	@param Количество удаляемых элементов.
	*/
	double erase_end(Container::size_type range){
		Container cont( _generator.begin(), _generator.end() );
		// не пользуемся обратными итераторами, чтобы избежать накладных расходов 
		// на приведение к обычному итератору
		Container::iterator it = --cont.end();
		Container::size_type count = range;
		{
			while(count){
				Container::iterator ers = it;		
				--count; --it;
				cont.erase(ers);
			}
			Counter t;
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

	/** @brief	Поиск элемента по мапу.
	Необходимо использовать метод класса.
	*/
	double find(){
		Container cont( _generator.begin(), _generator.end() );
		{
			Counter t;
			Container::iterator it = cont.find( _generator.random_data() ); 
		}
		return Counter::get_last_count();
	}

	/** @brief	Поиск подпоследовательности по контейнеру.
	Не имеет смысла в мапе
	*/
	double search(){
		return -1;
	}

	/** @brief Копирование контейнера.
	*/
	double copy(){
		Container cont1( _generator.begin(), _generator.end() );
		{
			Container cont2( _generator.begin(), _generator.end() );	
			{
				Counter t;
				cont1 = cont2;
			}

		}
		return Counter::get_last_count();
	}

	/** @brief Модификация элементов контейнера.
	*/
	double transform(){
		Container cont( _generator.begin(), _generator.end() );
		Container::iterator it = cont.begin();
		{
			Counter t;
			while(it != cont.end()){
				(*it).second *= 2;
				++it;
			}
		}
		return Counter::get_last_count();
	}

	/** @brief Реверс элементов контейнера не имеет смысла в мапе
	*/
	double reverse(){
		return -1;
	}

	/** @brief Перемешивание элементов в мапе не имеет смысла,
	т.к. мап всегда отсортирован.
	*/
	double random_shuffle(){
		{
			Counter t;
		}
		return Counter::get_last_count();
	}

	/** @brief Сортировка в мапе не имеет смысла,
	т.е. мап всегда отсортирован.
	*/
	double sort(){
		return -1;
	}

	/** @brief Сортировка в мапе не имеет смысла,
	т.е. мап всегда отсортирован.
	*/
	double stable_sort(){
		return -1;
	}

	/** @brief Поиск в отсортированном мапе не имеет смысла,
	т.е. мап всегда отсортирован.
	*/
	double find_sorted(){
		return find();
	}

	/** @brief Слияние отсортированых последовательностей не имеет смысла в мапе.
	*/
	double merge_sorted(){
		return -1;
	}

	/** @brief Логическая сумма (объединение) не имеет смысла в мапе.
	*/
	double set_union(){
		return -1;
	}

	/** @brief Логическое умножение (пересечение) не имеет смысла в мапе.
	*/
	double set_intersection(){
		return -1;
	}
};

// -------------------  -------------------
/**@brief 
Специализация для мультимапа  */
template < typename Element, typename Counter, typename Generator >
class container_test< assiciative_multimap_impl, Element, Counter, Generator > 
	: public container_test< assiciative_map_impl, Element, Counter, Generator > {
public:
	container_test(unsigned count, bool sorted = false) 
		: container_test< assiciative_map_impl, Element, Counter, Generator >(count, sorted){}
};

#endif //_PERF_TEST_SPECIAL_