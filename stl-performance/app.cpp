#include "stl.h"
#include <fstream>
#include <cstdlib>
#include <boost/program_options.hpp>
#include <time.h>

#include "random_data_generator.h"
#include "clock_count.h"
#include "special.h"
#include "test_result.h"
#include "command_line_params.h"

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;

/** @file
«десь происход€т все действи€ по тестированию и анализу. 
‘актически, единственный cpp-файл в проекте.
*/

// todo : remove after refactoring
const char* names[] = { 
	"creation",
	"fill",
	"push_data",
	"add_range",
	"iterate",
	"iterate_erase",
	"erase_some",
	"erase_begin",
	"erase_end",
	"erase",
	"find",
	"search",
	"copy",
	"transform",
	"reverse",
	"random_shuffle",
	"sort",
	"stable_sort",
	"find_sorted",
	"merge_sorted",
	"union",
	"intersection",
};

typedef msec_count CurrentTimer;

// определени€ типов дл€ тестовых наборов стандартных контейнеров
typedef container_test< serial_vector_impl, int
	, CurrentTimer , static_data_generator > vector_test;

typedef container_test< serial_list_impl, int
	, CurrentTimer ,static_data_generator > list_test;

typedef container_test< serial_deque_impl, int
	, CurrentTimer ,static_data_generator > deque_test;

typedef container_test< assiciative_set_impl, int
	, CurrentTimer ,static_data_generator > set_test;

typedef container_test< assiciative_multiset_impl, int
	, CurrentTimer ,static_data_generator > multiset_test;

typedef container_test< assiciative_map_impl, int
	, CurrentTimer ,static_pair_generator > map_test;

typedef container_test< assiciative_multimap_impl, int
	, CurrentTimer ,static_pair_generator > multimap_test;

/**@brief ‘уектор дл€ форматированного вывода результатов тестировани€
*/
struct formatted_output{
	const string testval;
	char delim;
	formatted_output(const string& val, char d):testval(val),delim(d){}
	void operator()(test_executor& t) const {
		std::cout << t.get_test_value(testval) << delim;
	}
};

template<typename T>
void callback_type(test_executor test_objects[]){
	test_objects[cont_vector].perform_some_tests<vector_test, T>(get_params()._test_names);
	test_objects[cont_list].perform_some_tests<list_test, T>(get_params()._test_names);
	test_objects[cont_deque].perform_some_tests<deque_test, T>(get_params()._test_names);
	test_objects[cont_set].perform_some_tests<set_test, T>(get_params()._test_names);
	test_objects[cont_multiset].perform_some_tests<multiset_test, T>(get_params()._test_names);
	test_objects[cont_map].perform_some_tests<map_test, T>(get_params()._test_names);
	test_objects[cont_multimap].perform_some_tests<multimap_test, T>(get_params()._test_names);
}

/**@brief «апуск набора тестов
*/
void play_test_executor(){
	
	// --- «адать дл€ каждого тестового набора размер ---
	test_executor test_objects[containers_type_count];
	for_each( test_objects, test_objects + containers_type_count
		, std::bind2nd( mem_fun_ref(&test_executor::set_count), get_params()._size ) );

	const char* cont_names[containers_type_count] = 
	{"vector","list","deque","set","m-set"
	,"map","m-map","h-set","h-map"};

	// --- ¬се ли тесты нужно запускать? ---
	if( get_params()._all_tests ){
		size_t sz = sizeof(names)/sizeof(char*);
		get_params()._test_names.assign(names, names + sz);
	}

	// --- запуск с отображением прогресса или молча (при записи в файл) ---
	if( get_params()._progress ){
		callback_type<test_executor::progress_cb>(test_objects);
	}
	else{
		callback_type<test_executor::opaque_cb>(test_objects);
	}

	// --- ¬ыведем условные имена контейнеров, если надо ---
	if( get_params()._show_container_names ){
		std::cout << '\t' ;
		for(int i = 0 ; i < containers_type_count ; i++) {
			std::cout << cont_names[i] << '\t';
		}
		std::cout << std::endl;
	}

	// --- ‘орматированный вывод результатов ---
	struct test_result_out{
		test_executor* _result;

		test_result_out(test_executor* r) : _result(r){}
		 void operator()(const std::string& s){
			if( get_params()._show_test_names ){
				std::cout << s << '\t';
			}
			const formatted_output fo(s, '\t');
			std::for_each( _result, _result + containers_type_count, fo );
			std::cout << std::endl;
		}
	};

	test_result_out result_output(test_objects);
	for_each( get_params()._test_names.begin()
		, get_params()._test_names.end(), result_output );

}

/**@brief ќбработка параметров командной строки и выполнение основных операций
*/
void actions(const command_line_params& p){
	if( p._help ){
		cout << get_params().options_descript() << endl;
		exit(EXIT_SUCCESS);
	}
	if( p._version ) {
		cout << "0.1b" << endl;
		exit(EXIT_SUCCESS);
	}

	// todo : progress|file out - в разные потоки
	// --- тестирование и вывод результатов в файл ---
	if( !get_params()._outfile.empty() ){
		
		// примен€ть ли режим добавлени€ в файл?
		ios_base::open_mode md = ( p._append ) ? (ios_base::out | ios_base::app) : (ios_base::out);
		
		ofstream file(get_params()._outfile.c_str(), md );
		streambuf* strm_buffer = cout.rdbuf();
		cout.rdbuf (file.rdbuf());
		play_test_executor();
		cout.rdbuf (strm_buffer);

		// todo : cross-platform this!
		// --- ѕередача результатов во внешнее приложение дл€ анализа ---
		if( !get_params()._application.empty() ){

#ifdef _WIN32
			PROCESS_INFORMATION pi = {};
			STARTUPINFOA si = {};
			string app_start;
			app_start.append("\"");
			app_start.append(get_params()._application.c_str());
			app_start.append("\"");

			app_start.append(" ");
			app_start.append("\"");
			app_start.append( get_params()._outfile.c_str() );
			app_start.append("\"");

			BOOL b = CreateProcessA(
				NULL,
				(LPSTR)app_start.c_str(),
				NULL,	// lpProcessAttributes
				NULL,	// lpThreadAttributes
				FALSE,	// bInheritHandles
				NULL,	// dwCreationFlags
				NULL,	// lpEnvironment
				NULL,	// lpCurrentDirectory
				&si,	// LPSTARTUPINFO
				&pi );	// LPPROCESS_INFORMATION
#else
			//FILE* b = popen(app_start.c_str(), "wb");
#endif

		}
	}
	// --- тестирование и вывод результатов в консоль ---
	else{
		play_test_executor();
	}
	exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[]){
	// -------------------  -------------------
	try {
		get_params().read_params(argc, argv);
		actions( get_params() );
	}
	// -------------------  -------------------
	// исключени€ boost::program_options сообщают о неправильном использовании 
	// командной строки, выведем подсказку
	catch(const po::error& e){
		cout << "Program option error: " << e.what() << endl;
		cout << get_params().options_descript() << endl;
	}
	catch(const std::exception& e) {
		cout << "Exception occur: " << e.what() << endl;
	}
	catch(...) {
		cout << "Unknown exception occur" << endl;
	}
	// -------------------  -------------------
	return 0;
}
