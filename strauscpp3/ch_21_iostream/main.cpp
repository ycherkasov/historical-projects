// этот хедер содержит forward declarations для всех шаблонов
// его использование сокращает время компиляции
#include <iosfwd>
#include "streams.h"
#include "formatted_user_input.h"
using namespace std;

void show_standard_streams(){

	// Стандартные потоки
	show_standard_streams_out();
	show_standard_streams_input();

}


void show_file_streams(){
	show_fstream();
}



int main(){
	
	// for testing pieces of code
	//test_me();

	// uncomment one by one
	//show_standard_streams();
	//show_file_streams();
	//show_formatted_streams();
	//show_string_streams();
	//show_wide_char_locales();
	show_formatted_overloads();
	return 0;
}
