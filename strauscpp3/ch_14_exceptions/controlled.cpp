#include "controlled.h"
#include <iostream>
#include <exception>

using std::cout;
using std::endl;
using std::vector;
using std::exception;

// --------------------------------------
//  онтролируемый конструктор - перехватывает все исключени€
// даже в списке инициализации ( и передает их далее )
controlled::controlled() try : _vect(1000){
	for( int i = 0 ; i < 1000 ; i++ )
		_vect.push_back(1);	

	throw exception("constructor exception");
}
catch(const exception& e) {
	cout << e.what() << endl;
}
catch(...){
	cout << "Unknown exception" << endl;
}

// --------------------------------------
// ƒеструкторы не должны кидать исключени€
// ѕо крайней мере они не должны выходить за его пределы
controlled::~controlled() throw() {}

// --------------------------------------
//  онтролируемый метод
void controlled::controlled_method() const throw(std::exception) try{
	throw exception("controlled method exception");
}
catch( std::exception& e ){
	cout << e.what() << endl;
}
