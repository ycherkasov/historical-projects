#ifndef _PERF_TEST_CMDLINE_PARAMS_ 
#define _PERF_TEST_CMDLINE_PARAMS_ 

#include <string>
#include <boost/program_options.hpp>

/** @file
Здесь содержится класс обработки параметров командной строки.
*/

namespace po = boost::program_options;

/**@brief 
Класс для обработки и хранения переметров, переданных в командной строке */
struct command_line_params {

	/**@brief Вывести справку */
	bool _help;

	/**@brief Вывести версию */
	bool _version;

	/**@brief Показывать прогресс выполнения тестов */
	bool _progress;

	/**@brief Выполнить все тесты */
	bool _all_tests;

	/**@brief Отображать условные имена тестов*/
	bool _show_test_names;

	/**@brief Отображать условные имена контейнеров */
	bool _show_container_names;
	
	/**@brief Дописывать в конец файла с результатами */
	bool _append;

	/**@brief Размер тестового набора (элементов)*/
	int _size;

	/**@brief Конфигурационный файл*/
	std::string _config;

	/**@brief Файл с результатами тестирования */
	std::string _outfile;

	/**@brief Внешнее приложение, которому можно передать данные для анализа */
	std::string _application;

	/**@brief Условные имена тестов, которые нужно выполнить (отменяет опцию "все тесты") */
	std::vector< std::string > _test_names;

	command_line_params() : desc("Allowed options")
		, _size(10000)
		, _help(false)
		, _version(false)
		, _progress(false)
		, _append(false)
		, _all_tests(false)
		, _show_test_names(false)
		, _show_container_names(false)
	{}

	void read_params(int argc, char* argv[] ){
		
		desc.add_options()
			("help,h", "print usage message")
			("version,v", "print version")
			("append,a", "append result to file")
			("progress,p", "show testing progress")
			("show-test-names,n", "show legend (test names)")
			("show-test-containers,c", "show legend (container names)")
			("all-tests,a", "perform all tests (by default)")
			("test-names,N", po::value< std::vector<std::string> >(&_test_names), "perform all tests (by default)")
			("size,s", po::value<int>(&_size), "set container size")
			("test-script,S", po::value(&_config), "set test scenario file")
			("out-file,o", po::value<std::string>(&_outfile), "set output file (without progress and legend output)")
			("run-application,r", po::value(&_application), "pass test result to application");

		// --- обработка параметров командной строки ---
		po::variables_map vm;
		po::store(parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		set_flag(vm, _help, "help");
		set_flag(vm, _all_tests, "all-tests");
		set_flag(vm, _append, "append");
		set_flag(vm, _version, "version");
		set_flag(vm, _progress, "progress");
		set_flag(vm, _show_test_names, "show-test-names");
		set_flag(vm, _show_container_names, "show-test-containers");

		// --- если осуществляем вывод в файл - 
		// выводим только результарующую информацию по тестам, 
		// без оформления в читаемый вид
		if(_outfile.empty()){
			_show_container_names = _show_test_names = _progress = true;
		}

	}

	/**@brief 
	Установка флагов */
	void set_flag(const po::variables_map& vm, bool& flag, const char* str){
		if (vm.count(str)) {  
			flag = true;
		}
	}

	po::options_description& options_descript(){return desc;}

private:
	po::options_description desc;
};

static command_line_params& get_params(){
	static command_line_params p;
	return p;
}

#endif //_PERF_TEST_CMDLINE_PARAMS_ 