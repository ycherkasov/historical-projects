#include "rsdn_example.h"
#include <exception>
#include <string>
#include <iostream>
using std::exception;
using std::cout;
using std::endl;

class E : public std::exception {
private:

	// хранить std::string в объекте-исключении нельзя
	// т.к. при копировании он может бросить исключение
	// std::string const disc__;
	std::string const& disc__;

public:
	E(std::string const &disc_) : disc__(disc_) {
		cout << "created E" << endl;
	};
	virtual ~E() throw() {
		cout << "deleted E" << endl;
	};

	virtual const char* what() const throw() {
		return disc__.c_str();
	};
};


int test_ex() {

	try {
		throw E("exception");
    } 
	//catch (E e) {
	//	cout << e.what() << endl;
	//} 
	catch (exception const e) {
		cout << e.what() << endl;
	} 
	catch (...) {
		cout << "ex" << endl;
	};

	return 0;
}
