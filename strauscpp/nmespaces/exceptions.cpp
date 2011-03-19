#include "exceptions.h"

#include <limits>
#include <iostream>
using std::numeric_limits;



char int_to_char(int i){

	// numeric_limits - см. —траус 22.2
	if( ( numeric_limits<char>::max() < i ) || ( numeric_limits<char>::min() > i ) )
		throw range_error_t(i);
	return i;
}

void show_exceptions(){

	try{
		char a = int_to_char(35);
		a = int_to_char(5689);
	}
	// ловим по ссылке
	catch (const range_error_t& e){	
		// исключение считаетс€ обработанным с момента входа
		// в блок catch
		// Ќеперехваченные исключени€ или исключени€,
		// брошенные из блока catch, передаютс€ наверх
		std::cout << "Unable convert to char " << e._range << std::endl;
	}

	try{
		char a = int_to_char(45);
		a = int_to_char(123564);
	}
	// ловим по значению - вызоветс€ конструктор копировани€
	// так лучше не делать
	catch ( const range_error_t e ){
		std::cout << "Unable convert to char " << e._range << std::endl;
	}
	//catch(...) нежелателен, т.к.:
	//1. он без повторного бросани€ не в состо€нии вывести конкретное сообщение об ошибке.
	//2. ¬ зависимости от опций компил€ции он может ловить SEH исключени€, которые ловить не следует.
	//3. он может ловить исключени€, которые не должны ловитьс€ пользовательским кодом 
	// (boost::tread_interrupted, преднамеренно не наследник std::exception).
	//4. он может поймать исключение, которое брошено по указателю и требует освобождени€, 
	// но без повторного бросани€ не сможет его освободить.


}