#include <iostream>
#include <typeinfo>

using namespace std;

/*
1. Basic types (architecture-denendent and independent)
2. Constant (const, constexpr)
3. Curly braces init
4. Auto, auto examples, auto & generic programming
5. For, range-for
6. enum, enum class
7. Class invariant (negative length example)
8. Static assertions
*/

// 1. Basic types (architecture-denendent and independent)
// 2. Constant (const, constexpr)
// 3. Curly braces init
void show_basic_types(){

	// new initialisators
	double d1 = 2.3;
	//double d{2.3};

	// const have to be initialized
	const int ic = 17;
	const double dc = 10.0;
	// constexpr evaluates in compile time that all parts of expression is const
	// used as additional check or for the performance
#if 0
	constexpr int ce_itn = 18;
	// ok - dc is const
	constexpr double = sqrt(dc); 

	// error - d1 is not const
	constexpr double = sqrt(dc); 
#endif
}

// 4. Auto, auto examples, auto & generic programming
void show_auto(){
	//We use auto where we don’t have a specific reason to mention the type explicitly. ‘‘Specific reasons’’ include:
	//* The definition is in a large scope where we want to make the type clearly visible to readers of our code.
	//* We want to be explicit about a variable’s range or precision (e.g.,doublerather thanfloat)
	
	auto b = true;
	cout << typeid(b).name() << '\n';
	auto ch = 'x';
	cout << typeid(ch).name() << '\n';
	auto l = 123;
	cout << typeid(l).name() << '\n';
	auto d = 1.23;
	cout << typeid(d).name() << '\n';
	auto s = "string";
	cout << typeid(s).name() << '\n';
	auto expr = sqrt(2.5);
	cout << typeid(expr).name() << '\n';
}

// 5. For, range-for
void show_for(){
	int arr[] = {1,2,3,4,5,6};

	// range-for-statement
	// for loops thet traverse a sequence

	// by value
	for(auto x : arr)
	{
		cout << x << endl;
	}

	// by reference
	for(auto& x : arr)
	{
		cout << x << endl;
	}

}

// 6. enum, enum class
void show_enum(){

	// Strong-typed enum
	enum class Color{
		red,
		green,
		yellow
	};

	// Can not be assigned other type
	Color c = Color::red;
	// error!
	//c = 1;

	// classic enum
	enum TrafficLight{
		red,
		yellow,
		green
	};
	TrafficLight tl = TrafficLight::red;
	// TODO: difference?
	//tl = static_cast<Color>(1);
}

// Invarinat is a checked statement in moment of object creationn
// (should be done in constructor)
// e.g. length should be non-negative

void show_static_assert(){

	// compile-time check (statement should be true)
	static_assert( 4 <= sizeof(int), "Integers are too small" );
}

// is the program’s return value to the system.  
// If no value is returned, the system will receive a value indicating successful completion.  
// A nonzero value from main() indicates failure.
int main() {
	show_basic_types();
	show_auto();
	show_for();
	show_enum();
	return 0;
}