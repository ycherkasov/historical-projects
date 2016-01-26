/** @file test_result.h
«десь определены обобщенные операции по выполнениею тестов и обработке результатов.
*/


#ifndef _PERF_TEST_RESULT_ 
#define _PERF_TEST_RESULT_ 

#include <boost/assert.hpp>
#include <boost/progress.hpp>
#include "stl.h"
#include "command_line_params.h"

using namespace std;

/**@brief 
–еализаци€ обертки дл€ идентификации тестового метода
¬ зависимости от набора переметров хранитс€ указатель на метод шаблонного класса*/
template <typename TesterClass>
struct test_attributes{

	// ”казатель на метод, не принимающий параметров
	typedef double(TesterClass::* method1_t)();

	// ”казатель на метод, принимающий размер
	typedef double(TesterClass::* method2_t)(size_t);

	test_attributes(method1_t m) : m1(m), m2(){}
	test_attributes(method2_t m) : m1(), m2(m){}
	method1_t m1;
	method2_t m2;
};

/**@brief 
√лобальна€ инициализаци€ набора названий тестов и ассоциированных с ними операций
–еализовано в виде статического синглтона */
template<typename Type>
std::map<std::string, test_attributes<Type> >& get_test_ids(){

	typedef test_attributes<Type> mytestattr;

	// ѕара "условное им€ теста - указатель на метод шаблонного класса набора тестов"
	static std::map<std::string, mytestattr> m;

	// все имена условные, могут передаватьс€ из командной строки
	if( m.empty() ) {
		m.insert( std::make_pair("creation", mytestattr( &Type::creation ) ) );
		m.insert( std::make_pair("fill", mytestattr( &Type::fill ) ) );
		m.insert( std::make_pair("push_data", mytestattr( &Type::push_data ) ) );
		m.insert( std::make_pair("add_range", mytestattr( &Type::add_range ) ) );
		m.insert( std::make_pair("iterate", mytestattr(  &Type::iterate ) ) );
		m.insert( std::make_pair("iterate_erase", mytestattr( &Type::iterate_erase ) ) );
		m.insert( std::make_pair("erase_some", mytestattr( &Type::erase_some) ) );
		m.insert( std::make_pair("erase_begin", mytestattr( &Type::erase_begin) ) );
		m.insert( std::make_pair("erase_end", mytestattr( &Type::erase_end) ) );
		m.insert( std::make_pair("erase", mytestattr( &Type::erase ) ) );
		m.insert( std::make_pair("find", mytestattr( &Type::find ) ) );
		m.insert( std::make_pair("search", mytestattr( &Type::search ) ) );
		m.insert( std::make_pair("copy", mytestattr( &Type::copy ) ) );
		m.insert( std::make_pair("transform", mytestattr( &Type::transform ) ) );
		m.insert( std::make_pair("reverse", mytestattr( &Type::reverse ) ) );
		m.insert( std::make_pair("random_shuffle", mytestattr( &Type::random_shuffle ) ) );
		m.insert( std::make_pair("sort", mytestattr( &Type::sort ) ) );
		m.insert( std::make_pair("stable_sort", mytestattr( &Type::stable_sort ) ) );
		m.insert( std::make_pair("find_sorted", mytestattr( &Type::find_sorted ) ) );
		m.insert( std::make_pair("merge_sorted", mytestattr( &Type::merge_sorted ) ) );
		m.insert( std::make_pair("set_union", mytestattr( &Type::set_union ) ) );
		m.insert( std::make_pair("set_intersection", mytestattr( &Type::set_intersection ) ) );
	}
	return m;
}

/**@brief 
¬ данном классе выполн€ютс€ тесты и производитс€ первична€ обработка результатов  */
class test_executor{
private:

	/**@brief –езультаты тестов  */
	std::map<std::string, double> _test_results;

	/**@brief  освенный вызов метода тестового набора 
	дл€ методов без параметров */
	template <typename TesterClass>
	double perform_test_func( TesterClass& tclass
		, double (TesterClass::* method_p)() ){
			return (tclass.*method_p)();
	}

	/**@brief  освенный вызов метода тестового набора 
	дл€ методов с папаметром size_t */
	template <typename TesterClass>
	double perform_test_func( TesterClass& tclass
		, double (TesterClass::* method_p)(size_t)
		, size_t val){
			return (tclass.*method_p)(val);
	}

	/**@brief –азмер тестового набора и размер его значимой части (обычно 0.1 от общего)  */
	unsigned _count;
	unsigned _partly;
public:

	test_executor(unsigned c = 10000) : _count(c), _partly(c/10){}

	void set_count(unsigned c){
		_count = c;
		_partly = _count/10;
	}

	typedef int(*cont_test_callback)(int);

	double get_test_value(const std::string& test_id){
		map<string, double>::iterator it = _test_results.find(test_id);
		if(it != _test_results.end())
			return (*it).second;
		else
			return -1;
	}

	template<typename Type>
	void get_all_test_names(std::vector< std::string >& t){
		typedef std::map<std::string, test_attributes<Type> > testattr;
		t.clear();
		testattr::iterator it_ = get_test_ids<Type>().begin();
		while(it_ != get_test_ids<Type>().end()) {
			t.push_back( (*it_).first );
			++it_;
		}
	}
	

	// ѕрозрачный callback - ничего не делает
	struct opaque_cb{
		opaque_cb(size_t){}
		unsigned operator()(unsigned t){return t;}
	};

	// Callback с отображением прогресса
	struct progress_cb{
		progress_cb(size_t op_count):pd( static_cast<unsigned long>(op_count)
			, std::cout, "\n%\t ", "\t ", "Complete:" ){}
		boost::progress_display pd;
		unsigned operator()(unsigned t){++pd;return t;}
	};

	template<typename Type>
	void perform_test(const std::string& s){
		std::map<std::string, test_attributes<Type> > m = get_test_ids<Type>();
		std::map<std::string, test_attributes<Type> >::iterator it =
			m.find(s);
		
		// Ќет теста с таким именем
		if( it == m.end() )
			return;
		// debug
		//cout << "Test: " << s << endl;
		
		test_attributes<Type> t = (*it).second;
		
		if(t.m1){
			test_attributes<Type>::method1_t m = t.m1;
			Type ct(_count);
			double d = perform_test_func<Type>( ct, m );
			_test_results.insert( std::make_pair( s, d )  );
		}
		else if(t.m2){
			test_attributes<Type>::method2_t m = t.m2;
			Type ct(_count);
			double d = perform_test_func<Type>( ct, m, _partly );
			_test_results.insert( std::make_pair( s, d )  );
		}
	}

	template <typename Type, typename CallbackObj>
	void perform_some_tests( const std::vector<std::string>& str_vect ){

		typedef test_attributes<Type> mytestattr;
		std::map<std::string, mytestattr> m = get_test_ids<Type>();

		Type ct(_count);
		if( get_params()._progress ){
			std::cout << std::endl << "Perform test set for " 
				<< typeid(Type::Container).name() << std::endl;
		}

		CallbackObj cb( str_vect.size() );
		unsigned i = 0;
		std::vector<std::string>::const_iterator it = str_vect.begin();
		while( it != str_vect.end() ){
			perform_test<Type>(*it);
			cb(i++);
			++it;
		}
	}

};

#endif//_PERF_TEST_RESULT_ 