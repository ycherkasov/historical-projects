// Реализация обобщенных алгоритмов методами шаблонов

// ------------------- Реализация через указатель на функцию -------------------
namespace alg_func_pointer{
	// Реализуем алгоритм "копировать если больше"
	template< typename InputIterator, typename OutputIterator, typename T >
	void copy_if_greater(InputIterator first, InputIterator last, OutputIterator out, const T& val){
		while( first != last )	{
			// если больше, скопировать в out
			if(*first > val){
				*out = *first;
				++out;
			}
			++first;
		}
	}

	// Алгоритм не универсален в том плане, что условие жестко зашито в тело функции
	// Попробуем реализовать указателем на функцию и семейство предикатов
	template<typename T>
	bool less_func(const T& a,const T& b){return a > b;}

	template<typename T>
	bool greater_func(const T& a,const T& b){return a < b;}

	template< typename InputIterator, typename OutputIterator, typename T >
	void copy_if_greater(InputIterator first, InputIterator last, OutputIterator out
		,bool comparer( const T& a, const T& b ) , const T& val){
			while( first != last )	{
				// если соответствует условию 'comparer', скопировать в out
				if( comparer(val, *first) ){
					*out = *first;
					++out;
				}
				++first;
			}
	}

	// Или так:
	template< typename InputIterator, typename OutputIterator, typename Predicate, typename T >
	void copy_if_greater_pred(InputIterator first, InputIterator last, OutputIterator out
		,Predicate comparer, const T& val){
			while( first != last )	{
				// если соответствует условию 'comparer', скопировать в out
				if( comparer(val, *first) ){
					*out = *first;
					++out;
				}
				++first;
			}
	}
	// Недостатки алгоритма - жестко заданный параметром T val тип не поддается неявному приведению
	// Жесткий тип предиката ставнения
}


namespace alg_functor{
	// ------------------- Реализация через функтор -------------------
	// Преимущества реализации через функтор:
	// - можно перегрузить operator() несколько раз
	// - можно инициализировать, сохранять состояние и вводить любые дополнительные операции
	// - можно наследовать и делать operator() виртуальным

	// Функтор-предикат без параметра
	struct is_odd{
		bool operator()(const int d){
			return !(d%2);
		}
	};

	// Функтор-предикат с параметром
	struct greater_then{
		int _val;
		greater_then(int val) : _val(val){}
		bool operator()(const int i){
			return i > _val;
		}
	};

	// Функтор-шаблон с параметром
	template <typename T>
	struct less_then{
		T _val;
		less_then() : _val(){}
		less_then(const T& val) : _val(val){}
		bool operator()(const int i){
			return i < _val;
		}
	};

	template< typename InputIterator, typename OutputIterator, typename Predicate >
	void copy_if(InputIterator first, InputIterator last, OutputIterator out
		,Predicate comparer){
			while( first != last )	{
				// если соответствует условию 'comparer', скопировать в out
				if( comparer(*first) ){
					*out = *first;
					++out;
				}
				++first;
			}
	}
}

