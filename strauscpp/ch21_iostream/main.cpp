#include "streams.h"

using namespace std;

void show_standard_streams(){

	// Стандартные потоки
	//show_standard_streams_out();
	show_standard_streams_input();

}

void show_file_streams(){
	show_fstream();
}

void show_formatted_streams(){
	//stream_flags();
	stream_methods();
}

int main(){
	//show_standard_streams();
	//show_file_streams();
	show_formatted_streams();
	return 0;
}