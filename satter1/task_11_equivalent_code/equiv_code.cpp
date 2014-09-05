#include <iostream>

using namespace std;

// Task 1.16 From Satter 1
// What equivalet code could mean

/*
1. What can do the following code?

func( a++ );

What could be different in the following pieces of code?

a:
func( a++ );

b:
func( a );
a++;

*/

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// What could be func
#if 1

// assume a int
static int a = 1;

// 1. Macro
#define _MACRO_

// 2. Function
//#define _FUNCTION_

// 3. Object
//#define _OBJECT_

// 4. Typename
//#define _TYPENAME_

#endif

//////////////////////////////////////////////////////////////////////////
// MACRO
#ifdef _MACRO_

// Could be anything:
// a. 1 time
#define func(x) x

// b. 5 times
//#define func(x) (x,x,x,x,x)

// c. do nothing
//#define func(x)

#endif // _MACRO_

//////////////////////////////////////////////////////////////////////////
// FUNCTION
#ifdef _FUNCTION_

void func(int i){
    cout << "i = " << i << endl;
};

#endif // _FUNCTION_


//////////////////////////////////////////////////////////////////////////
// OBJECT (state could be changed)
#ifdef _OBJECT_

struct func_t{
    void operator()(int i){
        cout << "i = " << i << endl;
    }
};

static func_t func;

#endif // _OBJECT_



//////////////////////////////////////////////////////////////////////////
// TYPENAME
#ifdef _TYPENAME_
struct func
{
    func(int i):_i(i){
        cout << "i = " << _i << endl;
    }
    int _i;
};
#endif // _TYPENAME_




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// What could be a

//////////////////////////////////////////////////////////////////////////
// MACRO
// Acould be anything

//////////////////////////////////////////////////////////////////////////
// OBJECT
// should be defined
// T T::operator++(int)
// Including ITERATOR!

//////////////////////////////////////////////////////////////////////////
// BASE TYPE, ADDRESS


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/* Side-effects in case of

func(a);
a++;

*/

// 1. In the second case exception could be thrown => a++ will never be called
/*
void func(int i){
throw std::runtime_error("You never increment");
}
*/


// 2. a could be iterator and coudl be invalidated in
/*
vector<int> v;
// ...
void func(vector<int>::iterator& i){
v.erase(i);
// i could be invalidated!
// next a++ call causes UB!
}
*/


int main(){

    func(a++);
    cout << "a = " << a << endl;
    return 0;
}

