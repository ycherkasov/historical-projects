#ifndef _PERF_TEST_CMDLINE_PARAMS_ 
#define _PERF_TEST_CMDLINE_PARAMS_ 

#include <string>
#include <boost/program_options.hpp>

/** @file
����� ���������� ����� ��������� ���������� ��������� ������.
*/

namespace po = boost::program_options;

/**@brief 
����� ��� ��������� � �������� ����������, ���������� � ��������� ������ */
struct command_line_params {

	/**@brief ������� ������� */
	bool _help;

	/**@brief ������� ������ */
	bool _version;

	/**@brief ���������� �������� ���������� ������ */
	bool _progress;

	/**@brief ��������� ��� ����� */
	bool _all_tests;

	/**@brief ���������� �������� ����� ������*/
	bool _show_test_names;

	/**@brief ���������� �������� ����� ����������� */
	bool _show_container_names;
	
	/**@brief ���������� � ����� ����� � ������������ */
	bool _append;

	/**@brief ������ ��������� ������ (���������)*/
	int _size;

	/**@brief ���������������� ����*/
	std::string _config;

	/**@brief ���� � ������������ ������������ */
	std::string _outfile;

	/**@brief ������� ����������, �������� ����� �������� ������ ��� ������� */
	std::string _application;

	/**@brief �������� ����� ������, ������� ����� ��������� (�������� ����� "��� �����") */
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

		// --- ��������� ���������� ��������� ������ ---
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

		// --- ���� ������������ ����� � ���� - 
		// ������� ������ �������������� ���������� �� ������, 
		// ��� ���������� � �������� ���
		if(_outfile.empty()){
			_show_container_names = _show_test_names = _progress = true;
		}

	}

	/**@brief 
	��������� ������ */
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